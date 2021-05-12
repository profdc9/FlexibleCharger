
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

#include "mini-printf.h"

#define SHUTDOWN_CNTRL 4

#define VOLTAGESET 10
#define CURRENTSET 9
#define DRAINBASE 5
#define VOLTAGEADC A0
#define CURRENTADC A1

#define BUTTON1 6
#define BUTTON2 7
#define BUTTON3 8

#define ARDUINORX 1
#define ARDUINOTX 0

#define NUM_BUTTONS 3

const uint8_t buttonPin[NUM_BUTTONS] = {BUTTON1, BUTTON2, BUTTON3};
static uint8_t constStateButton[NUM_BUTTONS];
volatile uint8_t curStateButton[NUM_BUTTONS];
volatile bool pushedButton[NUM_BUTTONS];

#define LCD_EDISP A2
#define LCD_RS A3
#define LCD_DB4 A4
#define LCD_DB5 A5
#define LCD_DB6 2
#define LCD_DB7 3

LiquidCrystal lcd(LCD_RS, LCD_EDISP, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

#define EE_STRUCT_ADDRESS 0x00
#define VALID_CALIB_MAGIC 0xC001ACE5

#define EE_CHARGE_PROGRAM_STRUCT 0x100u
#define EE_CHARGE_PROGRAM_SEPARATION 0x80u
#define EE_NUMBER_OF_CHARGE_PROGRAMS 12
#define VALID_PROGRAM_MAGIC 0xf001400b

typedef enum _charge_program_type
{
  CONSTANT_VOLTAGE_CONSTANT_CURRENT = 0,
  PULSED_RECOVERY_MODE = 1
} charge_program_type;

const char *program_type_list[]= { "CV/CC", "Pulsed Recovery", NULL };

typedef struct _program_constant_voltage_constant_current
{
  uint16_t constant_voltage;
  uint16_t constant_current;
} program_constant_voltage_constant_current;
  
typedef struct _program_pulsed_recovery_mode
{
  uint16_t pulse_voltage;
  uint16_t pulse_current;
  uint16_t pulse_length;
  uint16_t pulse_interval;
  uint16_t recovery_voltage;
  uint16_t target_voltage;
  uint16_t trickle_charge;
} program_pulsed_recovery_mode;

typedef union _charge_programs
{
  program_constant_voltage_constant_current cvcc;
  program_pulsed_recovery_mode prm;
} charge_programs;

typedef struct _charge_program_struct
{
  uint32_t valid_program;
  charge_program_type cpt;
  charge_programs cp;
} charge_program_struct;

typedef struct _calibration_struct
{
  uint32_t valid_calib;
  float voltage_dac_scaling;
  float current_dac_scaling;
  float a_cal;
  float b_cal;
  float c_cal;
  float d_cal;
} calibration_struct;

calibration_struct cs;

bool read_charge_program_struct(uint8_t prno, charge_program_struct *cps)
{
  if (prno >= EE_NUMBER_OF_CHARGE_PROGRAMS) return false;
  EEPROM.get(EE_CHARGE_PROGRAM_STRUCT + prno * EE_CHARGE_PROGRAM_SEPARATION, *cps);
  return (cps->valid_program == VALID_PROGRAM_MAGIC);
}

bool write_charge_program_struct(uint8_t prno, charge_program_struct *cps)
{
  if (prno >= EE_NUMBER_OF_CHARGE_PROGRAMS) return false;
  cps->valid_program = VALID_PROGRAM_MAGIC;
  EEPROM.put(EE_CHARGE_PROGRAM_STRUCT + prno * EE_CHARGE_PROGRAM_SEPARATION, *cps);
}

bool is_calibration_valid(void)
{
  return cs.valid_calib == VALID_CALIB_MAGIC;
}

bool isButtonDown(uint8_t b)
{
  return !curStateButton[b];
}

void waitButtonUp(uint8_t b)
{
  while (curStateButton[b] == LOW);
  pushedButton[b] = false;
}

bool getPushedButton(uint8_t b)
{
  if (pushedButton[b])
  {
    pushedButton[b] = false;
    return true;
  }
  return false;
}

void clearButtons(void)
{
  for (uint8_t b = 0; b < NUM_BUTTONS; b++)
    pushedButton[b] = false;
}

bool getPushedAndWaitButton(uint8_t b)
{
  bool state = getPushedButton(b);
  if (state)
  {
    waitButtonUp(b);
    clearButtons();
  }
  return state;
}


void debounce_buttons(void)
{
  for (uint8_t b = 0; b < NUM_BUTTONS; b++)
  {
    uint8_t state = digitalRead(buttonPin[b]);
    if (curStateButton[b] != state)
    {
      if ((++constStateButton[b]) >= 20)
      {
        curStateButton[b] = state;
        if (state == LOW) pushedButton[b] = true;
        constStateButton[b] = 0;
      }
    } else constStateButton[b] = 0;
  }
}

volatile uint8_t skipcounter = 0;
volatile uint32_t countv = 0;

ISR(TIMER1_OVF_vect)
{
  skipcounter++;
  if (skipcounter < 16) return;
  skipcounter = 0;
  countv++;
  debounce_buttons();
}

void shutdown_output()
{
  digitalWrite(SHUTDOWN_CNTRL, HIGH);
  analogWrite(VOLTAGESET,0);
  analogWrite(CURRENTSET,0);
}

void output_voltage_current(uint16_t voltage, uint16_t current)
{
  digitalWrite(SHUTDOWN_CNTRL, HIGH);  
  analogWrite(VOLTAGESET,voltage);
  analogWrite(CURRENTSET,current);
  digitalWrite(SHUTDOWN_CNTRL, LOW);  
}

void setup() {
  pinMode(SHUTDOWN_CNTRL, OUTPUT);
  pinMode(VOLTAGESET, OUTPUT);
  pinMode(CURRENTSET, OUTPUT);
  shutdown_output();
  pinMode(DRAINBASE, OUTPUT);
  digitalWrite(DRAINBASE, LOW);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(VOLTAGEADC, INPUT);
  pinMode(CURRENTADC, INPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);

  /* Set up FAST PWM, 10-bit */
  noInterrupts();           // disable all interrupts
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10)  ;
  TCCR1B = _BV(CS10) | _BV(WGM12);
  TIMSK1 |= _BV(TOIE1);  /* enable timer overflow interrupt */
  interrupts();  

  analogReference(DEFAULT);
  EEPROM.get(EE_STRUCT_ADDRESS,cs);
}

void accumulate_voltage_and_current(uint16_t avgs, float *volts, float *current)
{
  uint32_t voltavg = 0;
  uint32_t curavg = 0;
  float avgfloat = avgs;
  while (avgs>0)
  {
    avgs--;
    voltavg += analogRead(VOLTAGEADC);
    curavg += analogRead(CURRENTADC);
  }
  *volts = ((float)voltavg)/avgfloat;
  *current = ((float)curavg)/avgfloat;
}

uint8_t do_menu(const char *items[], const char *prompt, uint8_t item)
{
  for (;;)
  {
    lcd.clear();
    lcd.print(prompt);
    lcd.setCursor(0, 1);
    lcd.print(items[item]);
    for (;;)
    {
      if ((getPushedButton(0)) && (item > 0)) 
      {
        item--;
        break;
      }
      if ((getPushedButton(1)) && (items[item+1] != NULL))
      {
        item++;
        break;
      }
      if (getPushedAndWaitButton(2))
      {
        return item;
      }
    }
  }
}

uint32_t inc_number(uint8_t consec)
{
  if (consec > 45) return 30000;
  if (consec > 40) return 10000;
  if (consec > 35) return 3000;
  if (consec > 30) return 1000;
  if (consec > 25) return 300;
  if (consec > 20) return 100;
  if (consec > 15) return 30;
  if (consec > 10) return 10;
  if (consec > 5) return 3;
  return 1; 
}

uint32_t enter_number(uint32_t n, uint32_t minimum_number, uint32_t maximum_number, const char *format, uint16_t divisor, const char *prompt)
{
  char s[20];
  uint8_t consec = 0;
  for (;;)
  {
    lcd.clear();
    lcd.print(prompt);
    mini_snprintf(s,sizeof(s)-1,format,n/divisor,n%divisor);
    lcd.setCursor(0,1);
    lcd.print(s);
    delay(consec ? 250 : 500);
    for (;;) {
        if (isButtonDown(0))
        {
          if (n < (minimum_number + inc_number(consec)))
            n = minimum_number;
          else
            n -= inc_number(consec);
          if (consec < 50) consec++;
          break;
        } else if (isButtonDown(1))
        {
          if (n >= (maximum_number - inc_number(consec)))
            n = maximum_number;
          else
            n += inc_number(consec);
          if (consec < 50) consec++;
          break;
        } else if (isButtonDown(2)) 
        {
          waitButtonUp(2);
          clearButtons();
          return n;
        } 
        consec = 0;
    }
  }
}

bool show_messages(const char *message1, const char *message2)
{
    
  clearButtons();
  lcd.clear();
  lcd.print(message1);
  lcd.setCursor(0,1);
  lcd.print(message2);
  for (;;) {
     if (getPushedButton(0)) 
     {
       waitButtonUp(0);
       return false;
     }  
     if (getPushedButton(2))
     {
       waitButtonUp(2);
       return true;
     }  
  }
}

bool go_or_abort(const char *message)
{
  return show_messages(message, "Rt cont,lft abrt");
}

#define OPEN_CIRCUIT_DAC_VOLTAGE_SAMPLE 512
#define OPEN_CIRCUIT_DAC_CURRENT_SAMPLE 100
#define LOAD_CIRCUIT_DAC_VOLTAGE_SAMPLE 256
#define LOAD_CIRCUIT_DAC_CURRENT_SAMPLE 1023
#define SHORT_CIRCUIT_DAC_VOLTAGE_SAMPLE 100
#define SHORT_CIRCUIT_DAC_CURRENT_SAMPLE 512
#define NUMBER_OF_AVERAGES 256
#define SENSE_RESISTOR_VALUE_OHMS (0.1f)

void calculate_load_current_and_voltage(calibration_struct *ns, float voltsamp, float currentsamp, float *truevolts, float *truecurrent)
{
  *truevolts = ns->a_cal * voltsamp + ns->b_cal * currentsamp;
  *truecurrent = ns->c_cal * voltsamp + ns->d_cal * currentsamp;
}

void do_calibrate(void)
{
  calibration_struct ns;

  float resistance_load;
  float open_ckt_voltage;
  float load_ckt_voltage;

  float open_voltage, open_current;
  float load_voltage, load_current;
  float short_voltage, short_current;
  float actual_voltage, actual_current;

  if (!go_or_abort("Open ckt test"))
    return;
  output_voltage_current(OPEN_CIRCUIT_DAC_VOLTAGE_SAMPLE,  OPEN_CIRCUIT_DAC_CURRENT_SAMPLE);
  delay(1000);
  accumulate_voltage_and_current(NUMBER_OF_AVERAGES, &open_voltage, &open_current);
  if (!go_or_abort("Measure voltage and enter"))
  {
    shutdown_output();
    return;
  }
  shutdown_output();
  open_ckt_voltage = ((float)enter_number(1343, 0, 3000, "%02lu.%02lu", 100, "Enter open ckt voltage"))*0.01f;
  ns.voltage_dac_scaling = ((float)OPEN_CIRCUIT_DAC_VOLTAGE_SAMPLE) / open_ckt_voltage;

  resistance_load = ((float)enter_number(5111, 1000, 30000, "%02lu.%03lu", 1000, "Load resistance"))*0.001f;
  if (!go_or_abort("Load test"))
    return;
  output_voltage_current(LOAD_CIRCUIT_DAC_VOLTAGE_SAMPLE, LOAD_CIRCUIT_DAC_CURRENT_SAMPLE);
  delay(1000);
  accumulate_voltage_and_current(NUMBER_OF_AVERAGES, &load_voltage, &load_current);
  if (!go_or_abort("Measure voltage and enter"))
  {
    shutdown_output();
    return;
  }
  shutdown_output();  
  load_ckt_voltage = ((float)enter_number(676, 0, 3000, "%02lu.%02lu", 100, "Enter load ckt voltage"))*0.01f;

  float divisor = load_current * open_voltage - open_current * load_voltage;
  ns.a_cal = (load_current * open_ckt_voltage - open_current * load_ckt_voltage) / divisor;
  ns.b_cal = (open_voltage * load_ckt_voltage - load_voltage * open_ckt_voltage) / divisor;
  ns.c_cal = -open_current * load_ckt_voltage / (divisor * resistance_load);
  ns.d_cal = open_voltage * load_ckt_voltage / (divisor * resistance_load);

  if (!go_or_abort("Short test"))
    return;

  output_voltage_current(SHORT_CIRCUIT_DAC_VOLTAGE_SAMPLE, SHORT_CIRCUIT_DAC_CURRENT_SAMPLE);
  delay(1000);
  accumulate_voltage_and_current(NUMBER_OF_AVERAGES, &short_voltage, &short_current);
  shutdown_output();  
  calculate_load_current_and_voltage(&ns, short_voltage, short_current, &actual_voltage, &actual_current);

  ns.current_dac_scaling = ((float)(SHORT_CIRCUIT_DAC_CURRENT_SAMPLE)) / actual_current;  
  ns.valid_calib = VALID_CALIB_MAGIC;
  
  EEPROM.put(EE_STRUCT_ADDRESS, ns);
  cs = ns;
  go_or_abort("Calib finished");
}

bool check_calibration(void)
{
  if (!is_calibration_valid())
  {
     show_messages("No calibration","Press left button");
     return false;
  }
  return true;
}

#define VOLTAGE_LIMITED_DAC_VOLTAGE_SAMPLE 512
#define VOLTAGE_LIMITED_DAC_CURRENT_SAMPLE 1023

#define CURRENT_LIMITED_DAC_VOLTAGE_SAMPLE 1023
#define CURRENT_LIMITED_DAC_CURRENT_SAMPLE 512

void string_float(char *s, size_t n, const char *format, float f)
{
  char sn=' ';
  if (f<0)
  {
    f=-f;
    sn='-';
  }
  mini_snprintf(s,n,format,sn,(int16_t)f,(uint16_t)((f-floor(f))*1000.0f));
}

void display_voltage_and_current(float actual_voltage, float actual_current)
{
   char s[20];
   string_float(s,sizeof(s)-1,"V: %c%d.%03u",actual_voltage);
   lcd.clear();
   lcd.print(s);
   string_float(s,sizeof(s)-1,"A: %c%d.%03u",actual_current);
   lcd.setCursor(0,1);
   lcd.print(s);  
}

void acquire_and_display_voltage_current(void)
{
   float test_voltage, test_current;
   float actual_voltage, actual_current;
    
   accumulate_voltage_and_current(NUMBER_OF_AVERAGES, &test_voltage, &test_current);
   calculate_load_current_and_voltage(&cs, test_voltage, test_current, &actual_voltage, &actual_current);
   display_voltage_and_current(actual_voltage, actual_current);
}

void do_test_load(void)
{
  float test_voltage, test_current;
  float actual_voltage, actual_current;
  char s[20];
  
  if (!check_calibration()) return;
  if (!go_or_abort("Attach test load"))
    return;

  output_voltage_current(VOLTAGE_LIMITED_DAC_VOLTAGE_SAMPLE,  VOLTAGE_LIMITED_DAC_CURRENT_SAMPLE);
  delay(2500);  
  accumulate_voltage_and_current(NUMBER_OF_AVERAGES, &test_voltage, &test_current);
  shutdown_output();
 
  calculate_load_current_and_voltage(&cs, test_voltage, test_current, &actual_voltage, &actual_current);

  display_voltage_and_current(actual_voltage, actual_current);
  clearButtons();
  while (!getPushedAndWaitButton(2));
 
  output_voltage_current(CURRENT_LIMITED_DAC_VOLTAGE_SAMPLE, CURRENT_LIMITED_DAC_CURRENT_SAMPLE);
  delay(2500);  
  accumulate_voltage_and_current(NUMBER_OF_AVERAGES, &test_voltage, &test_current);
  shutdown_output();

  calculate_load_current_and_voltage(&cs, test_voltage, test_current, &actual_voltage, &actual_current);
 
  display_voltage_and_current(actual_voltage, actual_current);
  clearButtons();
  while (!getPushedAndWaitButton(2));
}

bool set_output_voltage_current(float voltage, float current, bool ignoremax = false)
{
   float dac_voltage = cs.voltage_dac_scaling * voltage;
   float dac_current = cs.current_dac_scaling * current;
   if (ignoremax)
   {
     if (dac_voltage > 1023.0f) dac_voltage = 1023.0f;
     if (dac_current > 1023.0f) dac_current = 1023.0f;
   } else if ((dac_voltage > 1023.0f) || (dac_current > 1023.0f)) return false;
   output_voltage_current((uint16_t) dac_voltage, (uint16_t) dac_current);
   return true;
}

void do_power_supply(void)
{
  float set_voltage = 0.0f;
  float set_current;
  bool output_off = false;
  uint32_t last_update = 0;

  if (!check_calibration()) return;
  for (;;)
  {
    if (set_voltage == 0.0f)
    {
       shutdown_output();
       set_voltage = ((float)enter_number(0, 0, 3000, "%02lu.%02lu", 100, "Set voltage"))*0.01f;
       set_current = ((float)enter_number(0, 0, 3000, "%02lu.%02lu", 100, "Set current"))*0.01f;
       if (!set_output_voltage_current(set_voltage, set_current))
       {
         show_messages("Current too high","Press left button");
         set_voltage = 0.0f;
         continue;
       }
       clearButtons();
    }
    if (getPushedAndWaitButton(0))
    {
       set_voltage = 0.0f;
       continue;
    }
    if (getPushedAndWaitButton(1))
    {
       if (output_off)
       {
           set_output_voltage_current(set_voltage, set_current);
           output_off = false;
       } else
       {
          shutdown_output();
          output_off = true;
       }
    } 
    if (getPushedAndWaitButton(2))
    {
       shutdown_output();
       break;
    }
    uint32_t current_update = millis();
    if ((current_update - last_update)>100)
    {
      last_update = current_update;
      if (output_off)
      {
        lcd.clear();
        lcd.print("Output off");
        lcd.setCursor(0,1);
        lcd.print("Middle button on");       
      } else
      {
        acquire_and_display_voltage_current();
      }
    }
  }
}

uint8_t select_program(void)
{
  uint8_t current_program = 0;
  charge_program_struct cps;
  for (;;) 
  {
    bool valid = read_charge_program_struct(current_program, &cps);
    lcd.clear();
    lcd.print("Select #");
    lcd.print(current_program+1);
    lcd.print(":");
    lcd.setCursor(0,1);
    lcd.print(valid ? program_type_list[cps.cpt] : "Empty list");
    for (;;)
    {
      if (getPushedAndWaitButton(0))
      {
        if (current_program > 0) current_program--;
        break;
      }
      if (getPushedAndWaitButton(1))
      {
        if (current_program < (EE_NUMBER_OF_CHARGE_PROGRAMS-1)) current_program++;
        break;
      }
      if (getPushedAndWaitButton(2))
        return current_program;
    }
  }
}

void do_execute_constant_voltage_constant_current(charge_program_struct *cps)
{
  bool output_off = false;
  uint32_t last_update = 0;
  
  float target_voltage_max = cps->cp.cvcc.constant_voltage * 0.01f;
  float target_voltage_min = target_voltage_max * 0.99f;
  float target_voltage_overshoot = target_voltage_max * 1.1f;

  float target_current = cps->cp.cvcc.constant_current * 0.01f;
  
  float test_voltage, test_current;
  float actual_voltage, actual_current;

  float servo_voltage = 0.0f, servo_current = target_current * 1.5f;
  for (;;)
  {
    uint32_t current_update = millis();
    if ((current_update - last_update)>100)
    {
      bool changed = false;
      accumulate_voltage_and_current(NUMBER_OF_AVERAGES, &test_voltage, &test_current);
      calculate_load_current_and_voltage(&cs, test_voltage, test_current, &actual_voltage, &actual_current);
      if (actual_current > target_current)
      {
        servo_voltage *= (0.9f + 0.1f * target_current / actual_current);
        changed = true;
      } else if (actual_voltage > target_voltage_max)
      {
        servo_voltage += 0.1f * (target_voltage_min - actual_voltage);
        changed = true;
      } else if (actual_voltage < target_voltage_min)
      {
        servo_voltage += 0.1f * (target_voltage_max - actual_voltage);
        if (servo_voltage > target_voltage_overshoot)
          servo_voltage = target_voltage_overshoot;
        changed = true;
      }
      
      if ((changed) && (!output_off))
        set_output_voltage_current(servo_voltage, servo_current);  

      last_update = current_update;
#if 0
         char s[20];
      string_float(s,sizeof(s)-1,"SV: %c%d.%03u",servo_voltage);
       lcd.clear();
      lcd.print(s);
       string_float(s,sizeof(s)-1,"SA: %c%d.%03u",servo_current);
      lcd.setCursor(0,1);
      lcd.print(s);  
#endif

      acquire_and_display_voltage_current();
      lcd.setCursor(13,1);
      lcd.print(output_off ? "off" : "on");
    }
    if (getPushedAndWaitButton(1))
    {
      if (output_off)
      {
         output_off = false;
         set_output_voltage_current(servo_voltage, servo_current);       
      } else
      {
         output_off = true;
         shutdown_output();
      }
    }
    if (getPushedAndWaitButton(2))
    { 
      shutdown_output();
      return;
    }
  }
}

void do_execute_pulsed_recovery_mode(charge_program_struct *cps)
{
  uint32_t last_update = 0;
  uint32_t number_of_pulses = 0;  
  bool pulsing = true;
  float test_voltage, test_current;
  float actual_voltage, actual_current;
  float recovery_voltage = cps->cp.prm.recovery_voltage * 0.01f;
  float pulse_voltage = cps->cp.prm.pulse_voltage * 0.01f;
  float pulse_current = cps->cp.prm.pulse_current * 0.01f;
  
  while (pulsing)
  {
    shutdown_output();
    for (uint16_t i=0;i<cps->cp.prm.pulse_interval;i++)
    {
      if (getPushedAndWaitButton(2))
        return;
      delayMicroseconds(1000);
    }
    for (uint8_t i=0;i<5;i++)
    {
      accumulate_voltage_and_current(NUMBER_OF_AVERAGES, &test_voltage, &test_current);
      calculate_load_current_and_voltage(&cs, test_voltage, test_current, &actual_voltage, &actual_current);
      if (actual_voltage < recovery_voltage) break;
      if (i == 4)
        pulsing = false;
    }
    uint32_t current_update = millis();
    if ((current_update - last_update)>100)
    {
      last_update = current_update;
      char s[20];
      string_float(s,sizeof(s)-1,"V: %c%d.%03u",actual_voltage);
      lcd.clear();
      lcd.print(s);
      lcd.setCursor(0,1);
      lcd.print("P: ");
      lcd.print(number_of_pulses);  
    }      
    if (pulsing)
    {
        set_output_voltage_current(pulse_voltage, pulse_current, true);  
        for (uint16_t i=0;i<cps->cp.prm.pulse_length;i++)
        {
          if (getPushedAndWaitButton(2))
          {
            shutdown_output();
            return;
          }
          delayMicroseconds(1000);
        }
        shutdown_output();
    }
    number_of_pulses++;
  }
  set_output_voltage_current(cps->cp.prm.target_voltage * 0.01f, cps->cp.prm.trickle_charge * 0.01f);  
  for (;;)
  {
    uint32_t current_update = millis();
    if ((current_update - last_update)>100)
    {
      last_update = current_update;
      acquire_and_display_voltage_current();
    } 
    if (getPushedAndWaitButton(2))
    {
      shutdown_output();
      return;
    }
  }
}

void do_execute_program(void)
{ 
  if (!check_calibration()) return;
  uint8_t program = select_program();  
  charge_program_struct cps;
  if (!read_charge_program_struct(program, &cps))
  {
     show_messages("Select valid prgm","Press left button");
     return;
  }
  if (!go_or_abort("Execute program?"))
    return;
  switch (cps.cpt)
  {
    case CONSTANT_VOLTAGE_CONSTANT_CURRENT:  do_execute_constant_voltage_constant_current(&cps); break;
    case PULSED_RECOVERY_MODE:  do_execute_pulsed_recovery_mode(&cps); break;
  }
}

void do_edit_constant_voltage_constant_current(charge_program_struct *cps)
{
  cps->cp.cvcc.constant_voltage = enter_number(cps->cp.cvcc.constant_voltage, 0, 3000, "%02lu.%02lu", 100, "Const voltage");
  cps->cp.cvcc.constant_current = enter_number(cps->cp.cvcc.constant_current, 0, 3000, "%02lu.%02lu", 100, "Const current");
}

void do_edit_pulsed_recovery_mode(charge_program_struct *cps)
{
  cps->cp.prm.pulse_voltage = enter_number(cps->cp.prm.pulse_voltage, 0, 3000, "%02lu.%02lu", 100, "Pulsed voltage");
  cps->cp.prm.pulse_current = enter_number(cps->cp.prm.pulse_current, 0, 3000, "%02lu.%02lu", 100, "Pulsed current");
  cps->cp.prm.pulse_length = enter_number(cps->cp.prm.pulse_length, 0, 1000, "%05lu", 1, "Pulse length(ms)");
  cps->cp.prm.pulse_interval = enter_number(cps->cp.prm.pulse_interval, 0, 65000, "%05lu", 1, "Interval (ms)");
  cps->cp.prm.recovery_voltage = enter_number(cps->cp.prm.recovery_voltage, 0, 3000, "%02lu.%02lu", 100, "Recovery volts");
  cps->cp.prm.target_voltage = enter_number(cps->cp.prm.target_voltage, 0, 3000, "%02lu.%02lu", 100, "Target volts");
  cps->cp.prm.trickle_charge = enter_number(cps->cp.prm.trickle_charge, 0, 3000, "%02lu.%02lu", 100, "Trickle charge");
}

void do_change_program(void)
{
  uint8_t program = select_program();
  charge_program_struct cps;
  bool valid = read_charge_program_struct(program, &cps);
  uint8_t ptype = do_menu(program_type_list,"Select type",valid ? cps.cpt : 0);
  if ((!valid) || (ptype != cps.cpt))
  {
    memset(&cps,'\000',sizeof(cps));
    cps.cpt = (charge_program_type)ptype;
    if (!go_or_abort("Erase program?"))
     return;
  } else
  {
    if (!go_or_abort("Change program?"))
      return;
  }
  switch (cps.cpt)
  {
    case CONSTANT_VOLTAGE_CONSTANT_CURRENT:  do_edit_constant_voltage_constant_current(&cps); break;
    case PULSED_RECOVERY_MODE:  do_edit_pulsed_recovery_mode(&cps); break;
  }
  write_charge_program_struct(program, &cps);
}

const char *main_menu_items[]= { "Execute Program", "Power Supply", "Change Program", "Calibrate", "Test Load", NULL };

void loop() {
  uint8_t item;
  item = do_menu(main_menu_items,"Battery charger menu",0);
  switch (item)
  {
    case 0: do_execute_program();
            break;
    case 1: do_power_supply();
            break;
    case 2: do_change_program();
            break;
    case 3: do_calibrate();
            break;
    case 4: do_test_load();
            break;
  }
}
