<Qucs Schematic 0.0.22>
<Properties>
  <View=0,0,800,800,1,0,0>
  <Grid=10,10,1>
  <DataSet=ShuntCompensation.dat>
  <DataDisplay=ShuntCompensation.dpl>
  <OpenDisplay=1>
  <Script=ShuntCompensation.m>
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
  <OpAmp OP1 1 650 260 -26 42 0 0 "1e6" 1 "15 V" 0>
  <R R2 1 110 220 15 -26 0 1 "0.1 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <GND * 1 280 370 0 0 0 0>
  <GND * 1 110 290 0 0 0 0>
  <GND * 1 160 90 0 0 0 0>
  <GND * 1 320 400 0 0 0 0>
  <.DC DC1 1 90 490 0 47 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <GND * 1 530 480 0 0 0 0>
  <R R1 1 380 240 -26 15 0 0 "100k Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R3 1 380 350 -26 15 0 0 "27000000k Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R6 1 530 410 15 -26 0 1 "10k Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R5 1 690 380 -26 15 0 0 "2.2k Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <Idc I1 1 110 120 -67 -26 0 3 "3 A" 1>
  <R R4 1 380 160 -26 15 0 0 "420k Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <Vdc V1 1 280 310 18 -26 0 1 "0 V" 1>
</Components>
<Wires>
  <280 240 280 280 "" 0 0 0 "">
  <280 240 350 240 "" 0 0 0 "">
  <280 340 280 370 "" 0 0 0 "">
  <110 250 110 290 "" 0 0 0 "">
  <110 150 110 160 "" 0 0 0 "">
  <110 70 110 90 "" 0 0 0 "">
  <110 70 160 70 "" 0 0 0 "">
  <160 70 160 90 "" 0 0 0 "">
  <110 160 110 190 "" 0 0 0 "">
  <110 160 350 160 "" 0 0 0 "">
  <410 160 450 160 "" 0 0 0 "">
  <450 160 450 240 "" 0 0 0 "">
  <410 350 450 350 "" 0 0 0 "">
  <450 240 450 350 "" 0 0 0 "">
  <410 240 450 240 "" 0 0 0 "">
  <320 350 320 400 "" 0 0 0 "">
  <320 350 350 350 "" 0 0 0 "">
  <450 240 620 240 "" 0 0 0 "">
  <590 280 620 280 "" 0 0 0 "">
  <590 280 590 380 "" 0 0 0 "">
  <590 380 660 380 "" 0 0 0 "">
  <690 260 740 260 "" 0 0 0 "">
  <740 260 740 380 "" 0 0 0 "">
  <720 380 740 380 "" 0 0 0 "">
  <530 380 590 380 "" 0 0 0 "">
  <530 440 530 480 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
