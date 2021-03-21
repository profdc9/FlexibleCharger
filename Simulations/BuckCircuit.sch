<Qucs Schematic 0.0.22>
<Properties>
  <View=0,0,1545,950,1,0,0>
  <Grid=10,10,1>
  <DataSet=BuckCircuit.dat>
  <DataDisplay=BuckCircuit.dpl>
  <OpenDisplay=1>
  <Script=BuckCircuit.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <_BJT TIP42C_1 1 370 190 -26 -32 1 1 "pnp" 0 "5.65618e-10" 0 "1.24004" 0 "1.30331" 0 "1.46498" 0 "10" 0 "90.6071" 0 "27.1221" 0 "6.9828e-14" 0 "4" 0 "6.98934e-14" 0 "3.78125" 0 "50" 0 "2.83268" 0 "0.12691" 0 "0.234602" 0 "0.0927424" 0 "0.000666374" 0 "4.71382" 0 "01.93221e-10" 0 "0.4" 0 "0.259369" 0 "1.0962e-10" 0 "0.731968" 0 "0.23" 0 "0.799902" 0 "0" 0 "0.75" 0 "0.5" 0 "0.799995" 0 "9.99163e-9" 0 "4.41941" 0 "6.53488" 0 "0.001" 0 "1e-7" 0 "26.85" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <_BJT TIP41C_1 1 530 190 -26 -32 0 1 "npn" 0 "7.55826e-11" 0 "1.11221" 0 "1.5" 0 "0.526814" 0 "3.54059" 0 "100" 0 "1000" 0 "1e-8" 0 "2.18072" 0 "1e-8" 0 "1.63849" 0 "50" 0 "26.0542" 0 "0.1" 0 "0.1" 0 "0.0810556" 0 "0.0162111" 0 "4.56157" 0 "1.93296e-10" 0 "0.4" 0 "0.259503" 0 "1.09657e-10" 0 "0.730921" 0 "0.23" 0 "0.803085" 0 "0" 0 "0.75" 0 "0.5" 0 "0.8" 0 "1e-8" 0 "4.06972" 0 "7.1157" 0 "0.001" 0 "9.01013e-8" 0 "26.85" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <_BJT Q2N2222A_1 1 370 390 8 -26 0 0 "npn" 0 "8.11e-14" 0 "1" 0 "1" 0 "0.5" 0 "0.225" 0 "113" 0 "24" 0 "1.06e-11" 0 "2" 0 "0" 0 "2" 0 "205" 0 "4" 0 "0" 0 "0" 0 "0.137" 0 "0.343" 0 "1.37" 0 "2.95e-11" 0 "0.75" 0 "0.33" 0 "1.52e-11" 0 "0.75" 0 "0.33" 0 "1" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "3.97e-10" 0 "0" 0 "0" 0 "0" 0 "8.5e-08" 0 "26.85" 0 "0" 0 "1" 0 "1" 0 "0" 0 "1" 0 "1" 0 "0" 0 "1.5" 0 "3" 0 "1.11" 0 "26.85" 0 "1" 0>
  <GND * 1 620 350 0 0 0 0>
  <Diode D_1N5408_1 1 620 270 33 -2 0 3 "63n" 0 "1.7" 0 "53p" 0 "0.333" 0 "0.7" 0 "0.5" 0 "0" 0 "0" 0 "2" 0 "14.1m" 0 "4.32u" 0 "0" 0 "0" 0 "1" 0 "1" 0 "1k" 0 "10u" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "normal" 0>
  <R R5 1 520 260 -26 15 0 0 "33 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <GND * 1 370 500 0 0 0 0>
  <GND * 1 170 510 0 0 0 0>
  <VProbe Pr1 1 520 440 28 -16 1 0>
  <L L1 1 700 190 -26 10 0 0 "200 uH" 1 "" 0>
  <R R8 1 570 70 -26 15 0 0 "1 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <Vdc V1 1 60 310 18 -26 0 1 "24 V" 1>
  <GND * 1 60 390 0 0 0 0>
  <R R1 1 100 190 -26 15 0 0 "0.0 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R2 1 370 340 -130 -12 0 3 "220 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R4 1 220 260 -26 15 0 0 "100 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <IProbe Pr2 1 230 190 -26 16 0 0>
  <IProbe Pr3 1 250 70 -26 16 0 0>
  <.DC DC1 1 170 670 0 47 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <GND * 1 790 350 0 0 0 0>
  <GND * 1 910 350 0 0 0 0>
  <R R7 1 910 310 15 -26 0 1 "0 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R6 1 910 230 15 -26 0 1 "10 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <C C3 1 500 70 -26 -65 0 2 "100 pF" 1 "" 0 "neutral" 0>
  <Vrect V2 1 170 450 18 -26 0 1 "4 V" 1 "4 us" 1 "4 us" 1 "1 us" 0 "1 us" 0 "0 ns" 0>
  <R R3 1 270 390 -47 2 0 0 "1000 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <C C1 1 790 270 17 -26 0 1 "10 uF" 1 "" 0 "neutral" 0>
  <.TR TR1 1 30 550 0 79 0 0 "lin" 1 "0" 1 "200 us" 1 "40000" 0 "Trapezoidal" 0 "2" 0 "1 ns" 0 "1e-16" 0 "150" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "26.85" 0 "1e-3" 0 "1e-6" 0 "1" 0 "CroutLU" 0 "no" 0 "yes" 0 "0" 0>
</Components>
<Wires>
  <400 190 450 190 "" 0 0 0 "">
  <450 190 450 220 "" 0 0 0 "">
  <450 220 530 220 "" 0 0 0 "">
  <300 390 340 390 "" 0 0 0 "">
  <170 390 170 420 "" 0 0 0 "">
  <170 390 240 390 "" 0 0 0 "">
  <620 300 620 350 "" 0 0 0 "">
  <620 190 620 240 "" 0 0 0 "">
  <620 190 670 190 "" 0 0 0 "">
  <560 190 580 190 "" 0 0 0 "">
  <580 190 620 190 "" 0 0 0 "">
  <580 190 580 260 "" 0 0 0 "">
  <550 260 580 260 "" 0 0 0 "">
  <450 260 490 260 "" 0 0 0 "">
  <450 220 450 260 "" 0 0 0 "">
  <370 420 370 500 "" 0 0 0 "">
  <170 480 170 510 "" 0 0 0 "">
  <730 190 790 190 "" 0 0 0 "">
  <450 410 510 410 "" 0 0 0 "">
  <510 410 510 420 "" 0 0 0 "">
  <530 410 530 420 "" 0 0 0 "">
  <530 410 580 410 "" 0 0 0 "">
  <580 260 580 410 "" 0 0 0 "">
  <600 70 620 70 "" 0 0 0 "">
  <620 70 620 190 "" 0 0 0 "">
  <60 340 60 360 "" 0 0 0 "">
  <60 360 60 390 "" 0 0 0 "">
  <130 190 160 190 "" 0 0 0 "">
  <60 190 60 280 "" 0 0 0 "">
  <60 190 70 190 "" 0 0 0 "">
  <370 220 370 260 "" 0 0 0 "">
  <370 360 370 370 "" 0 0 0 "">
  <370 260 370 310 "" 0 0 0 "">
  <250 260 370 260 "" 0 0 0 "">
  <160 190 160 260 "" 0 0 0 "">
  <160 260 190 260 "" 0 0 0 "">
  <260 190 340 190 "" 0 0 0 "">
  <160 190 200 190 "" 0 0 0 "">
  <280 70 470 70 "" 0 0 0 "">
  <160 70 160 190 "" 0 0 0 "">
  <160 70 220 70 "" 0 0 0 "">
  <530 70 540 70 "" 0 0 0 "">
  <470 70 470 190 "" 0 0 0 "">
  <470 190 500 190 "" 0 0 0 "">
  <790 300 790 350 "" 0 0 0 "">
  <790 190 790 240 "" 0 0 0 "">
  <790 190 910 190 "OutputV" 860 120 50 "">
  <910 340 910 350 "" 0 0 0 "">
  <910 260 910 280 "" 0 0 0 "">
  <910 190 910 200 "" 0 0 0 "">
  <450 260 450 410 "" 0 0 0 "">
</Wires>
<Diagrams>
  <Rect 450 820 630 330 3 #c0c0c0 1 00 1 0 0.2 1 1 -0.1 0.5 1.1 1 -0.1 0.5 1.1 315 0 225 "" "" "">
	<"ngspice/tran.i(pr2)" #0000ff 0 3 0 0 0>
	<"ngspice/tran.i(pr3)" #ff0000 0 3 0 0 0>
  </Rect>
  <Rect 1036 406 452 296 3 #c0c0c0 1 00 1 0 0.0002 0.002 1 -5 10 30 1 -1 0.5 1 315 0 225 "" "" "">
	<"ngspice/tran.v(pr1)" #0000ff 0 3 0 0 0>
	<"ngspice/tran.v(outputv)" #ff0000 0 3 0 0 0>
  </Rect>
</Diagrams>
<Paintings>
</Paintings>
