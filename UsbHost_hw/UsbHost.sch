EESchema Schematic File Version 2
LIBS:UsbHost-rescue
LIBS:Antennas_kl
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Power_kl
LIBS:st_kl
LIBS:Tittar_kl
LIBS:Transistors_kl
LIBS:RF ICs
LIBS:memory
LIBS:UsbHost-cache
EELAYER 26 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title ""
Date "23 nov 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 5050 700  0    60   Italic 12
CPU
Text Notes 5600 5800 0    60   ~ 12
Indication
Text Notes 7800 8850 0    60   ~ 12
Power
$Comp
L GND #PWR01
U 1 1 5149BCE1
P 3100 4200
F 0 "#PWR01" H 3190 4180 30  0001 C CNN
F 1 "GND" H 3100 4120 30  0001 C CNN
F 2 "" H 3100 4200 60  0001 C CNN
F 3 "" H 3100 4200 60  0001 C CNN
	1    3100 4200
	1    0    0    -1  
$EndComp
Text Label 6650 2700 0    60   ~ 0
SWCLK
Text Label 6650 2600 0    60   ~ 0
SWDIO
$Comp
L GND #PWR04
U 1 1 5149A472
P 4600 4200
F 0 "#PWR04" H 4690 4180 30  0001 C CNN
F 1 "GND" H 4600 4120 30  0001 C CNN
F 2 "" H 4600 4200 60  0001 C CNN
F 3 "" H 4600 4200 60  0001 C CNN
	1    4600 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 5149A46C
P 2850 4200
F 0 "#PWR05" H 2940 4180 30  0001 C CNN
F 1 "GND" H 2850 4120 30  0001 C CNN
F 2 "" H 2850 4200 60  0001 C CNN
F 3 "" H 2850 4200 60  0001 C CNN
	1    2850 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5149A469
P 2550 4200
F 0 "#PWR06" H 2640 4180 30  0001 C CNN
F 1 "GND" H 2550 4120 30  0001 C CNN
F 2 "" H 2550 4200 60  0001 C CNN
F 3 "" H 2550 4200 60  0001 C CNN
	1    2550 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 5149A467
P 2050 4200
F 0 "#PWR07" H 2140 4180 30  0001 C CNN
F 1 "GND" H 2050 4120 30  0001 C CNN
F 2 "" H 2050 4200 60  0001 C CNN
F 3 "" H 2050 4200 60  0001 C CNN
	1    2050 4200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR08
U 1 1 5149A44F
P 2500 3350
F 0 "#PWR08" H 2500 3310 30  0001 C CNN
F 1 "+3.3V" H 2580 3380 30  0000 C CNN
F 2 "" H 2500 3350 60  0001 C CNN
F 3 "" H 2500 3350 60  0001 C CNN
	1    2500 3350
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR09
U 1 1 5149A448
P 1750 3150
F 0 "#PWR09" H 1750 3110 30  0001 C CNN
F 1 "+3.3V" H 1830 3180 30  0000 C CNN
F 2 "" H 1750 3150 60  0001 C CNN
F 3 "" H 1750 3150 60  0001 C CNN
	1    1750 3150
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR010
U 1 1 5149A441
P 2800 3650
F 0 "#PWR010" H 2800 3610 30  0001 C CNN
F 1 "+3.3V" H 2880 3680 30  0000 C CNN
F 2 "" H 2800 3650 60  0001 C CNN
F 3 "" H 2800 3650 60  0001 C CNN
	1    2800 3650
	-1   0    0    1   
$EndComp
$Comp
L C C4
U 1 1 5149A42D
P 2050 3900
F 0 "C4" H 1950 3800 50  0000 L CNN
F 1 "0.1u" H 1950 4000 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 2150 3750 28  0001 C BNN
F 3 "" H 2050 3900 60  0001 C CNN
	1    2050 3900
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5149A385
P 2550 3900
F 0 "C6" H 2450 3800 50  0000 L CNN
F 1 "0.1u" H 2450 4000 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 2650 3750 28  0001 C BNN
F 3 "" H 2550 3900 60  0001 C CNN
	1    2550 3900
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5149A370
P 3100 3900
F 0 "C8" H 3000 3800 50  0000 L CNN
F 1 "1u" H 3000 4000 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 3200 3750 28  0001 C BNN
F 3 "" H 3100 3900 60  0001 C CNN
	1    3100 3900
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5149A36D
P 2850 3900
F 0 "C7" H 2750 3800 50  0000 L CNN
F 1 "10n" H 2750 4000 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 2950 3750 28  0001 C BNN
F 3 "" H 2850 3900 60  0001 C CNN
	1    2850 3900
	1    0    0    -1  
$EndComp
NoConn ~ 3200 3000
Text Label 6650 2200 0    60   ~ 0
UART_TX
$Comp
L GND #PWR012
U 1 1 525CF187
P 1800 4200
F 0 "#PWR012" H 1890 4180 30  0001 C CNN
F 1 "GND" H 1800 4120 30  0001 C CNN
F 2 "" H 1800 4200 60  0001 C CNN
F 3 "" H 1800 4200 60  0001 C CNN
	1    1800 4200
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 525CF18D
P 1800 3900
F 0 "C3" H 1700 3800 50  0000 L CNN
F 1 "10u" H 1700 4000 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 1900 3750 28  0001 C BNN
F 3 "" H 1800 3900 60  0001 C CNN
	1    1800 3900
	1    0    0    -1  
$EndComp
Text Notes 11150 1400 0    60   ~ 0
Radio
$Comp
L CRYSTAL_H XTAL2
U 1 1 5265B349
P 9300 3150
F 0 "XTAL2" H 9250 3450 60  0000 C CNN
F 1 "27MHz" H 9300 3350 60  0000 C CNN
F 2 "Quartz:03225C4" H 9325 3275 39  0001 C CNN
F 3 "" H 9300 3150 60  0001 C CNN
	1    9300 3150
	1    0    0    -1  
$EndComp
$Comp
L C C14
U 1 1 5265B34F
P 9550 3400
F 0 "C14" H 9450 3300 50  0000 L CNN
F 1 "18pF" H 9450 3500 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 9650 3250 28  0001 C BNN
F 3 "" H 9550 3400 60  0001 C CNN
	1    9550 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 5265B355
P 9550 3700
F 0 "#PWR013" H 9640 3680 30  0001 C CNN
F 1 "GND" H 9550 3620 30  0001 C CNN
F 2 "" H 9550 3700 60  0001 C CNN
F 3 "" H 9550 3700 60  0001 C CNN
	1    9550 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5265B35B
P 9050 3700
F 0 "#PWR014" H 9140 3680 30  0001 C CNN
F 1 "GND" H 9050 3620 30  0001 C CNN
F 2 "" H 9050 3700 60  0001 C CNN
F 3 "" H 9050 3700 60  0001 C CNN
	1    9050 3700
	1    0    0    -1  
$EndComp
$Comp
L C C12
U 1 1 5265B361
P 9050 3400
F 0 "C12" H 8950 3300 50  0000 L CNN
F 1 "18pF" H 8950 3500 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 9150 3250 28  0001 C BNN
F 3 "" H 9050 3400 60  0001 C CNN
	1    9050 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 5265B367
P 9300 3700
F 0 "#PWR015" H 9390 3680 30  0001 C CNN
F 1 "GND" H 9300 3620 30  0001 C CNN
F 2 "" H 9300 3700 60  0001 C CNN
F 3 "" H 9300 3700 60  0001 C CNN
	1    9300 3700
	1    0    0    -1  
$EndComp
Text Label 9650 2500 0    60   ~ 0
CC_GDO2
$Comp
L +3.3V #PWR016
U 1 1 5265B39E
P 11750 2800
F 0 "#PWR016" H 11750 2760 30  0001 C CNN
F 1 "+3.3V" H 11830 2830 30  0000 C CNN
F 2 "" H 11750 2800 60  0001 C CNN
F 3 "" H 11750 2800 60  0001 C CNN
	1    11750 2800
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR017
U 1 1 5265B3A4
P 12000 2700
F 0 "#PWR017" H 12000 2660 30  0001 C CNN
F 1 "+3.3V" H 12080 2730 30  0000 C CNN
F 2 "" H 12000 2700 60  0001 C CNN
F 3 "" H 12000 2700 60  0001 C CNN
	1    12000 2700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR018
U 1 1 5265B3AA
P 12250 2600
F 0 "#PWR018" H 12250 2560 30  0001 C CNN
F 1 "+3.3V" H 12330 2630 30  0000 C CNN
F 2 "" H 12250 2600 60  0001 C CNN
F 3 "" H 12250 2600 60  0001 C CNN
	1    12250 2600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR019
U 1 1 5265B3B0
P 12500 2500
F 0 "#PWR019" H 12500 2460 30  0001 C CNN
F 1 "+3.3V" H 12580 2530 30  0000 C CNN
F 2 "" H 12500 2500 60  0001 C CNN
F 3 "" H 12500 2500 60  0001 C CNN
	1    12500 2500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR020
U 1 1 5265B3B6
P 12750 2400
F 0 "#PWR020" H 12750 2360 30  0001 C CNN
F 1 "+3.3V" H 12830 2430 30  0000 C CNN
F 2 "" H 12750 2400 60  0001 C CNN
F 3 "" H 12750 2400 60  0001 C CNN
	1    12750 2400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR021
U 1 1 5265B3BC
P 13000 2300
F 0 "#PWR021" H 13000 2260 30  0001 C CNN
F 1 "+3.3V" H 13080 2330 30  0000 C CNN
F 2 "" H 13000 2300 60  0001 C CNN
F 3 "" H 13000 2300 60  0001 C CNN
	1    13000 2300
	1    0    0    -1  
$EndComp
Text Label 9650 2400 0    60   ~ 0
CC_GDO0
Text Label 9650 2200 0    60   ~ 0
CC_CS
Text Label 9650 1900 0    60   ~ 0
CC_SCK
Text Label 9650 2100 0    60   ~ 0
CC_MISO
Text Label 9650 2000 0    60   ~ 0
CC_MOSI
$Comp
L CC1101 DD2
U 1 1 5265B3C7
P 10900 2550
F 0 "DD2" H 10450 3350 60  0000 C CNN
F 1 "CC1101" H 11200 3350 60  0000 C CNN
F 2 "QFN_DFN:QFN20" H 10900 2550 60  0001 C CNN
F 3 "" H 10900 2550 60  0001 C CNN
	1    10900 2550
	1    0    0    -1  
$EndComp
Text Label 12400 2100 0    60   ~ 0
RF2
Text Label 12400 1900 0    60   ~ 0
RF1
$Comp
L GND #PWR022
U 1 1 5265B3CF
P 12950 3300
F 0 "#PWR022" H 13040 3280 30  0001 C CNN
F 1 "GND" H 12950 3220 30  0001 C CNN
F 2 "" H 12950 3300 60  0001 C CNN
F 3 "" H 12950 3300 60  0001 C CNN
	1    12950 3300
	1    0    0    -1  
$EndComp
$Comp
L C C21
U 1 1 5265B3D5
P 12950 3050
F 0 "C21" H 12850 3150 50  0000 L CNN
F 1 "0.1u" H 12850 2950 50  0000 L CNN
F 2 "Capacitors:CAP_0603" H 12950 3050 60  0001 C CNN
F 3 "" H 12950 3050 60  0001 C CNN
	1    12950 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 5265B3DB
P 12450 3300
F 0 "#PWR023" H 12540 3280 30  0001 C CNN
F 1 "GND" H 12450 3220 30  0001 C CNN
F 2 "" H 12450 3300 60  0001 C CNN
F 3 "" H 12450 3300 60  0001 C CNN
	1    12450 3300
	1    0    0    -1  
$EndComp
$Comp
L C C19
U 1 1 5265B3E1
P 12450 3050
F 0 "C19" H 12350 3150 50  0000 L CNN
F 1 "0.1u" H 12350 2900 50  0000 L CNN
F 2 "Capacitors:CAP_0603" H 12450 3050 60  0001 C CNN
F 3 "" H 12450 3050 60  0001 C CNN
	1    12450 3050
	1    0    0    -1  
$EndComp
$Comp
L C C20
U 1 1 5265B3E7
P 12700 3050
F 0 "C20" H 12600 3150 50  0000 L CNN
F 1 "0.1u" H 12600 2950 50  0000 L CNN
F 2 "Capacitors:CAP_0603" H 12700 3050 60  0001 C CNN
F 3 "" H 12700 3050 60  0001 C CNN
	1    12700 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 5265B3ED
P 12700 3300
F 0 "#PWR024" H 12790 3280 30  0001 C CNN
F 1 "GND" H 12700 3220 30  0001 C CNN
F 2 "" H 12700 3300 60  0001 C CNN
F 3 "" H 12700 3300 60  0001 C CNN
	1    12700 3300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 5265B3F3
P 12200 3300
F 0 "#PWR025" H 12290 3280 30  0001 C CNN
F 1 "GND" H 12200 3220 30  0001 C CNN
F 2 "" H 12200 3300 60  0001 C CNN
F 3 "" H 12200 3300 60  0001 C CNN
	1    12200 3300
	1    0    0    -1  
$EndComp
$Comp
L C C18
U 1 1 5265B3F9
P 12200 3050
F 0 "C18" H 12100 3150 50  0000 L CNN
F 1 "0.1u" H 12100 2950 50  0000 L CNN
F 2 "Capacitors:CAP_0603" H 12200 3050 60  0001 C CNN
F 3 "" H 12200 3050 60  0001 C CNN
	1    12200 3050
	1    0    0    -1  
$EndComp
$Comp
L C C17
U 1 1 5265B3FF
P 11950 3050
F 0 "C17" H 11850 3150 50  0000 L CNN
F 1 "0.1u" H 11850 2900 50  0000 L CNN
F 2 "Capacitors:CAP_0603" H 11950 3050 60  0001 C CNN
F 3 "" H 11950 3050 60  0001 C CNN
	1    11950 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 5265B405
P 11950 3300
F 0 "#PWR026" H 12040 3280 30  0001 C CNN
F 1 "GND" H 11950 3220 30  0001 C CNN
F 2 "" H 11950 3300 60  0001 C CNN
F 3 "" H 11950 3300 60  0001 C CNN
	1    11950 3300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 5265B40B
P 11700 3300
F 0 "#PWR027" H 11790 3280 30  0001 C CNN
F 1 "GND" H 11700 3220 30  0001 C CNN
F 2 "" H 11700 3300 60  0001 C CNN
F 3 "" H 11700 3300 60  0001 C CNN
	1    11700 3300
	1    0    0    -1  
$EndComp
$Comp
L C C16
U 1 1 5265B411
P 11700 3050
F 0 "C16" H 11600 3150 50  0000 L CNN
F 1 "0.1u" H 11600 2950 50  0000 L CNN
F 2 "Capacitors:CAP_0603" H 11700 3050 60  0001 C CNN
F 3 "" H 11700 3050 60  0001 C CNN
	1    11700 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR028
U 1 1 5265B417
P 11550 3300
F 0 "#PWR028" H 11640 3280 30  0001 C CNN
F 1 "GND" H 11550 3220 30  0001 C CNN
F 2 "" H 11550 3300 60  0001 C CNN
F 3 "" H 11550 3300 60  0001 C CNN
	1    11550 3300
	1    0    0    -1  
$EndComp
$Comp
L TESTPOINT TP1
U 1 1 5265B41D
P 9500 2400
F 0 "TP1" H 9300 2400 60  0000 C CNN
F 1 "TESTPOINT" H 9475 2625 60  0001 C CNN
F 2 "PCB:TESTPOINT_1MM" H 9500 2400 60  0001 C CNN
F 3 "" H 9500 2400 60  0001 C CNN
	1    9500 2400
	1    0    0    -1  
$EndComp
$Comp
L TESTPOINT TP2
U 1 1 5265B423
P 9500 2500
F 0 "TP2" H 9300 2500 60  0000 C CNN
F 1 "TESTPOINT" H 9475 2725 60  0001 C CNN
F 2 "PCB:TESTPOINT_1MM" H 9500 2500 60  0001 C CNN
F 3 "" H 9500 2500 60  0001 C CNN
	1    9500 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 5265B429
P 10850 3700
F 0 "#PWR029" H 10940 3680 30  0001 C CNN
F 1 "GND" H 10850 3620 30  0001 C CNN
F 2 "" H 10850 3700 60  0001 C CNN
F 3 "" H 10850 3700 60  0001 C CNN
	1    10850 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 5265B42F
P 9850 3700
F 0 "#PWR030" H 9940 3680 30  0001 C CNN
F 1 "GND" H 9850 3620 30  0001 C CNN
F 2 "" H 9850 3700 60  0001 C CNN
F 3 "" H 9850 3700 60  0001 C CNN
	1    9850 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 5265B435
P 10100 3700
F 0 "#PWR031" H 10190 3680 30  0001 C CNN
F 1 "GND" H 10100 3620 30  0001 C CNN
F 2 "" H 10100 3700 60  0001 C CNN
F 3 "" H 10100 3700 60  0001 C CNN
	1    10100 3700
	1    0    0    -1  
$EndComp
$Comp
L C C15
U 1 1 5265B43B
P 9850 3400
F 0 "C15" H 9850 3500 50  0000 L CNN
F 1 "0.1u" H 9850 3300 50  0000 L CNN
F 2 "Capacitors:CAP_0603" H 9850 3400 60  0001 C CNN
F 3 "" H 9850 3400 60  0001 C CNN
	1    9850 3400
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5265B441
P 10100 3400
F 0 "R5" V 10180 3400 50  0000 C CNN
F 1 "56k" V 10100 3400 50  0000 C CNN
F 2 "Resistors:RES_0603" H 10100 3400 60  0001 C CNN
F 3 "" H 10100 3400 60  0001 C CNN
	1    10100 3400
	1    0    0    -1  
$EndComp
Text Notes 13150 1400 0    60   ~ 0
Matching circuit
Text Label 6550 1500 0    60   ~ 0
CC_GDO2
Text Label 6550 1600 0    60   ~ 0
CC_GDO0
Text Label 6550 1700 0    60   ~ 0
CC_CS
Text Label 6550 1800 0    60   ~ 0
CC_SCK
Text Label 6550 1900 0    60   ~ 0
CC_MISO
Text Label 6550 2000 0    60   ~ 0
CC_MOSI
$Comp
L BALUN868JOHANSON BLN1
U 1 1 526657F6
P 13400 2000
F 0 "BLN1" H 13150 2350 60  0000 C CNN
F 1 "0896BM15A0001" H 13400 2250 60  0000 C CNN
F 2 "Radio:BALUN_JOHANSON868" H 13400 2000 60  0001 C CNN
F 3 "" H 13400 2000 60  0000 C CNN
	1    13400 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 52665933
P 13500 2500
F 0 "#PWR033" H 13590 2480 30  0001 C CNN
F 1 "GND" H 13500 2420 30  0001 C CNN
F 2 "" H 13500 2500 60  0001 C CNN
F 3 "" H 13500 2500 60  0001 C CNN
	1    13500 2500
	1    0    0    -1  
$EndComp
$Comp
L L L1
U 1 1 52665AF6
P 14350 1900
F 0 "L1" V 14250 1760 40  0000 C CNN
F 1 "5.6nH" V 14250 1900 40  0000 C CNN
F 2 "Inductors:IND_0402" V 14410 1910 40  0001 C CNN
F 3 "~" H 14350 1900 60  0000 C CNN
	1    14350 1900
	0    1    1    0   
$EndComp
Text Label 6650 2300 0    60   ~ 0
UART_RX
NoConn ~ 6450 2100
$Comp
L GND #PWR034
U 1 1 526E3E25
P 2300 4200
F 0 "#PWR034" H 2390 4180 30  0001 C CNN
F 1 "GND" H 2300 4120 30  0001 C CNN
F 2 "" H 2300 4200 60  0001 C CNN
F 3 "" H 2300 4200 60  0001 C CNN
	1    2300 4200
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 526E3E2B
P 2300 3900
F 0 "C5" H 2200 3800 50  0000 L CNN
F 1 "0.1u" H 2200 4000 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 2400 3750 28  0001 C BNN
F 3 "" H 2300 3900 60  0001 C CNN
	1    2300 3900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR035
U 1 1 526E3E32
P 2250 3250
F 0 "#PWR035" H 2250 3210 30  0001 C CNN
F 1 "+3.3V" H 2330 3280 30  0000 C CNN
F 2 "" H 2250 3250 60  0001 C CNN
F 3 "" H 2250 3250 60  0001 C CNN
	1    2250 3250
	-1   0    0    1   
$EndComp
Wire Wire Line
	6450 2600 6650 2600
Wire Wire Line
	3100 4100 3100 4200
Wire Wire Line
	2050 4100 2050 4200
Wire Wire Line
	6450 2200 6650 2200
Wire Wire Line
	2550 4100 2550 4200
Wire Wire Line
	2850 4100 2850 4200
Wire Wire Line
	6450 2700 6650 2700
Wire Wire Line
	1800 4100 1800 4200
Wire Wire Line
	9550 3200 9550 2800
Wire Wire Line
	9550 2800 10150 2800
Wire Wire Line
	10150 2700 9050 2700
Wire Wire Line
	9050 2700 9050 3200
Wire Wire Line
	9550 3600 9550 3700
Connection ~ 9550 3150
Wire Wire Line
	9150 3150 9050 3150
Connection ~ 9050 3150
Wire Wire Line
	10150 2200 9650 2200
Wire Wire Line
	10150 2000 9650 2000
Connection ~ 12950 2300
Wire Wire Line
	12950 2300 12950 2850
Connection ~ 12700 2400
Wire Wire Line
	12700 2400 12700 2850
Connection ~ 12450 2500
Wire Wire Line
	12450 2500 12450 2850
Connection ~ 12200 2600
Wire Wire Line
	12250 2600 11500 2600
Connection ~ 11950 2700
Wire Wire Line
	12000 2700 11500 2700
Wire Wire Line
	12950 3250 12950 3300
Wire Wire Line
	12450 3250 12450 3300
Wire Wire Line
	12700 3250 12700 3300
Wire Wire Line
	12200 3250 12200 3300
Wire Wire Line
	11950 3250 11950 3300
Connection ~ 11700 2800
Wire Wire Line
	11750 2800 11500 2800
Wire Wire Line
	11550 3300 11550 3000
Connection ~ 11550 3100
Wire Wire Line
	11500 3100 11550 3100
Wire Wire Line
	10150 2400 9500 2400
Wire Wire Line
	10850 3550 10850 3700
Wire Wire Line
	9850 3600 9850 3700
Wire Wire Line
	10150 3000 9850 3000
Wire Wire Line
	9850 3000 9850 3200
Wire Wire Line
	10100 3150 10100 3100
Wire Wire Line
	10100 3100 10150 3100
Wire Wire Line
	10100 3650 10100 3700
Wire Wire Line
	11550 3000 11500 3000
Wire Wire Line
	11700 3250 11700 3300
Wire Wire Line
	11700 2850 11700 2800
Wire Wire Line
	11950 2850 11950 2700
Wire Wire Line
	12200 2850 12200 2600
Wire Wire Line
	11500 2500 12500 2500
Wire Wire Line
	11500 2400 12750 2400
Wire Wire Line
	13000 2300 11500 2300
Wire Wire Line
	9650 1900 10150 1900
Wire Wire Line
	9650 2100 10150 2100
Wire Wire Line
	9500 2500 10150 2500
Wire Wire Line
	9050 3600 9050 3700
Wire Wire Line
	9300 3300 9300 3700
Wire Wire Line
	9450 3150 9550 3150
Wire Wire Line
	6550 1600 6450 1600
Wire Wire Line
	6450 1700 6550 1700
Wire Wire Line
	6550 1800 6450 1800
Wire Wire Line
	6450 1900 6550 1900
Wire Wire Line
	6550 2000 6450 2000
Wire Wire Line
	6450 1500 6550 1500
Wire Wire Line
	12950 1900 11500 1900
Wire Wire Line
	11500 2100 12950 2100
Wire Wire Line
	13500 2500 13500 2350
Wire Wire Line
	13400 2350 13400 2400
Wire Wire Line
	13400 2400 13600 2400
Connection ~ 13500 2400
Wire Wire Line
	13600 2400 13600 2350
Wire Wire Line
	13850 1900 14150 1900
Wire Notes Line
	450  4900 16050 4900
Wire Notes Line
	4150 4900 4150 11200
Wire Notes Line
	4150 8600 16000 8600
Wire Wire Line
	6650 2300 6450 2300
Connection ~ 2850 3650
Connection ~ 3100 3650
Connection ~ 2550 3350
Wire Wire Line
	14550 1900 14750 1900
Wire Wire Line
	3200 3650 2800 3650
Wire Wire Line
	2850 3700 2850 3650
Wire Wire Line
	3100 3500 3100 3700
Wire Wire Line
	2550 3700 2550 3350
Wire Wire Line
	2300 4100 2300 4200
Wire Wire Line
	2300 3700 2300 3250
Wire Wire Line
	2250 3250 3200 3250
Connection ~ 2300 3250
Wire Wire Line
	3200 3150 1750 3150
Wire Wire Line
	1800 3700 1800 3150
Connection ~ 1800 3150
Wire Wire Line
	2050 3700 2050 3150
Connection ~ 2050 3150
$Comp
L ANT_PCB_MONO ANT1
U 1 1 527152AC
P 14950 1900
F 0 "ANT1" H 14750 2150 60  0000 C CNN
F 1 "ANT_PCB_MONO" H 15000 2050 60  0000 C CNN
F 2 "Radio:ANT_868_SEIDHE" H 14950 1900 60  0001 C CNN
F 3 "~" H 14950 1900 60  0000 C CNN
	1    14950 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4200 4600 3950
Wire Wire Line
	4400 3950 4400 4050
Connection ~ 4600 4050
Wire Wire Line
	4500 3950 4500 4050
Connection ~ 4500 4050
Wire Wire Line
	4700 4050 4700 3950
Wire Wire Line
	4850 3950 4850 4150
Wire Wire Line
	3200 3350 2500 3350
Wire Wire Line
	3200 3500 3100 3500
NoConn ~ 6450 1400
NoConn ~ 3200 1500
NoConn ~ 3200 2300
NoConn ~ 3200 2400
NoConn ~ 3200 2500
NoConn ~ 3200 2600
NoConn ~ 3200 2700
NoConn ~ 3200 2800
NoConn ~ 6450 2800
NoConn ~ 3200 1600
NoConn ~ 3200 1800
NoConn ~ 3200 1900
NoConn ~ 3200 2000
NoConn ~ 3200 2100
NoConn ~ 3200 2200
$Comp
L NCP583SQ DA1
U 1 1 54721BD5
P 7100 10050
F 0 "DA1" H 6950 10400 60  0000 C CNN
F 1 "NCP583SQ" H 7100 10300 60  0000 C CNN
F 2 "SOT:SC82AB" H 7100 10050 60  0001 C CNN
F 3 "" H 7100 10050 60  0001 C CNN
	1    7100 10050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR039
U 1 1 54721BDB
P 8000 9950
F 0 "#PWR039" H 8000 9910 30  0001 C CNN
F 1 "+3.3V" H 8080 9980 30  0000 C CNN
F 2 "" H 8000 9950 60  0001 C CNN
F 3 "" H 8000 9950 60  0001 C CNN
	1    8000 9950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR040
U 1 1 54721BE1
P 7500 10500
F 0 "#PWR040" H 7590 10480 30  0001 C CNN
F 1 "GND" H 7500 10420 30  0001 C CNN
F 2 "" H 7500 10500 60  0001 C CNN
F 3 "" H 7500 10500 60  0001 C CNN
	1    7500 10500
	1    0    0    -1  
$EndComp
Text Notes 7700 9250 0    60   ~ 0
3.3V 150mA power regulator (one or another)
$Comp
L GND #PWR041
U 1 1 54721BE8
P 6200 10500
F 0 "#PWR041" H 6290 10480 30  0001 C CNN
F 1 "GND" H 6200 10420 30  0001 C CNN
F 2 "" H 6200 10500 60  0001 C CNN
F 3 "" H 6200 10500 60  0001 C CNN
	1    6200 10500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR042
U 1 1 54721BEE
P 7700 10500
F 0 "#PWR042" H 7790 10480 30  0001 C CNN
F 1 "GND" H 7700 10420 30  0001 C CNN
F 2 "" H 7700 10500 60  0001 C CNN
F 3 "" H 7700 10500 60  0001 C CNN
	1    7700 10500
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR043
U 1 1 54721BFA
P 5850 9950
F 0 "#PWR043" H 5850 9910 30  0001 C CNN
F 1 "+5V" H 5930 9980 30  0000 C CNN
F 2 "~" H 5850 9950 60  0000 C CNN
F 3 "~" H 5850 9950 60  0000 C CNN
	1    5850 9950
	-1   0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 54721C00
P 6200 10250
F 0 "C10" H 6100 10150 50  0000 L CNN
F 1 "1u" H 6100 10350 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 6300 10100 28  0001 C BNN
F 3 "~" H 6200 10250 60  0000 C CNN
	1    6200 10250
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 54721C06
P 7700 10250
F 0 "C13" H 7600 10150 50  0000 L CNN
F 1 "1u" H 7600 10350 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 7800 10100 28  0001 C BNN
F 3 "~" H 7700 10250 60  0000 C CNN
	1    7700 10250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 9950 8000 9950
Connection ~ 7700 9950
Wire Wire Line
	7700 10050 7700 9950
Connection ~ 6200 9950
Wire Wire Line
	7500 10150 7500 10500
Wire Wire Line
	7700 10450 7700 10500
Wire Wire Line
	6200 10450 6200 10500
Wire Wire Line
	5850 9950 6700 9950
Wire Wire Line
	6700 10150 6650 10150
Wire Wire Line
	6650 10150 6650 9950
Connection ~ 6650 9950
Text Notes 1550 8250 0    60   ~ 0
USB
Wire Wire Line
	6450 2400 6700 2400
Wire Wire Line
	6450 2500 6700 2500
NoConn ~ 6450 1300
Wire Wire Line
	6200 10050 6200 9950
NoConn ~ 6450 3200
NoConn ~ 6450 3300
NoConn ~ 6450 3400
$Comp
L C C2
U 1 1 56D47D35
P 2000 8800
F 0 "C2" H 2050 8700 50  0000 L CNN
F 1 "10n" H 2050 8900 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 1950 8650 28  0001 C BNN
F 3 "" H 2000 8800 60  0000 C CNN
	1    2000 8800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR046
U 1 1 56D47D3C
P 2000 8500
F 0 "#PWR046" H 2090 8480 30  0001 C CNN
F 1 "GND" H 2000 8420 30  0001 C CNN
F 2 "" H 2000 8500 60  0000 C CNN
F 3 "" H 2000 8500 60  0000 C CNN
	1    2000 8500
	1    0    0    -1  
$EndComp
Text Label 2150 9200 0    60   ~ 0
D-
Text Label 2150 9300 0    60   ~ 0
D+
$Comp
L USB_MINI_B XL3
U 1 1 56D47D45
P 1650 9300
F 0 "XL3" H 1450 9750 60  0000 C CNN
F 1 "USB_MINI_B" H 1550 9650 60  0000 C CNN
F 2 "Connectors:USBmicro_MOLEX_WM17142" H 1650 9300 60  0001 C CNN
F 3 "" H 1650 9300 60  0000 C CNN
	1    1650 9300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR047
U 1 1 56D47D4C
P 2000 9600
F 0 "#PWR047" H 2090 9580 30  0001 C CNN
F 1 "GND" H 2000 9520 30  0001 C CNN
F 2 "" H 2000 9600 60  0000 C CNN
F 3 "" H 2000 9600 60  0000 C CNN
	1    2000 9600
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR048
U 1 1 56D47D52
P 2400 9100
F 0 "#PWR048" H 2400 9060 30  0001 C CNN
F 1 "+5V" H 2480 9130 30  0000 C CNN
F 2 "" H 2400 9100 60  0000 C CNN
F 3 "" H 2400 9100 60  0000 C CNN
	1    2400 9100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 9400 2000 9400
Wire Wire Line
	2000 9400 2000 9600
Connection ~ 2000 9500
Wire Wire Line
	2000 9000 2000 9100
Connection ~ 2000 9100
Wire Wire Line
	2000 8600 2000 8500
Wire Wire Line
	1850 9200 2150 9200
Wire Wire Line
	1850 9300 2150 9300
Wire Wire Line
	2000 9500 1850 9500
Wire Wire Line
	2400 9100 1850 9100
Text Label 6700 2400 0    60   ~ 0
D-
Text Label 6700 2500 0    60   ~ 0
D+
$Comp
L C C22
U 1 1 56D4BB64
P 14100 2400
F 0 "C22" H 14150 2500 50  0000 L CNN
F 1 "2.7pF" H 14150 2300 50  0000 L CNN
F 2 "Capacitors:CAP_0402" H 14100 2400 60  0001 C CNN
F 3 "" H 14100 2400 60  0001 C CNN
	1    14100 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR049
U 1 1 56D4BB6A
P 14100 2650
F 0 "#PWR049" H 14190 2630 30  0001 C CNN
F 1 "GND" H 14100 2570 30  0001 C CNN
F 2 "" H 14100 2650 60  0001 C CNN
F 3 "" H 14100 2650 60  0001 C CNN
	1    14100 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	14100 2600 14100 2650
Wire Wire Line
	14100 1900 14100 2200
Connection ~ 14100 1900
$Comp
L R R3
U 1 1 56D523B0
P 5550 6750
F 0 "R3" V 5630 6600 50  0000 C CNN
F 1 "100R" V 5550 6750 50  0000 C CNN
F 2 "Resistors:RES_0603" V 5630 6850 28  0001 C CNN
F 3 "" H 5550 6750 60  0000 C CNN
	1    5550 6750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR050
U 1 1 56D523C5
P 7000 6750
F 0 "#PWR050" H 7090 6730 30  0001 C CNN
F 1 "GND" H 7000 6670 30  0001 C CNN
F 2 "" H 7000 6750 60  0000 C CNN
F 3 "" H 7000 6750 60  0000 C CNN
	1    7000 6750
	0    1    1    0   
$EndComp
Text Label 5000 6750 0    60   ~ 0
LED_R
Text Label 5000 6550 0    60   ~ 0
LED_G
Text Label 5000 6950 0    60   ~ 0
LED_B
Wire Wire Line
	5800 6950 5800 6900
Wire Wire Line
	5800 6900 6000 6900
Wire Wire Line
	5800 6750 6000 6750
Wire Wire Line
	5800 6550 5800 6600
Wire Wire Line
	5800 6600 6000 6600
Wire Wire Line
	6800 6750 7000 6750
Wire Wire Line
	6800 6600 6900 6600
Wire Wire Line
	6900 6600 6900 6900
Connection ~ 6900 6750
Wire Wire Line
	6900 6900 6800 6900
Wire Wire Line
	5300 6750 5000 6750
Wire Wire Line
	5300 6550 5000 6550
Wire Wire Line
	5300 6950 5000 6950
Text Label 2800 1300 0    60   ~ 0
LED_R
Text Label 2800 1400 0    60   ~ 0
LED_G
Text Label 2800 1700 0    60   ~ 0
LED_B
$Comp
L BUTTON SW1
U 1 1 56D52E00
P 5300 4050
F 0 "SW1" H 5100 4200 50  0000 C CNN
F 1 "BUTTON" H 5400 4200 50  0000 C CNN
F 2 "BtnsSwitches:BTN_4x4_SMD" H 5300 4050 60  0001 C CNN
F 3 "" H 5300 4050 60  0000 C CNN
	1    5300 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1700 2800 1700
Wire Wire Line
	3200 1300 2800 1300
Wire Wire Line
	2800 1400 3200 1400
$Comp
L STM32F072CxTx DD1
U 1 1 56D55AF6
P 4650 2500
F 0 "DD1" H 4150 3850 60  0000 C CNN
F 1 "STM32F072CxTx" H 4800 3850 60  0000 C CNN
F 2 "LQFP_TQFP:LQFP48" H 4650 2500 60  0001 C CNN
F 3 "" H 4650 2500 60  0000 C CNN
	1    4650 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 4050 4700 4050
$Comp
L GND #PWR051
U 1 1 56D58DEF
P 4850 4700
F 0 "#PWR051" H 4940 4680 30  0001 C CNN
F 1 "GND" H 4850 4620 30  0001 C CNN
F 2 "" H 4850 4700 60  0001 C CNN
F 3 "" H 4850 4700 60  0001 C CNN
	1    4850 4700
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 56D58DF5
P 4850 4400
F 0 "R2" V 4930 4400 50  0000 C CNN
F 1 "56k" V 4850 4400 50  0000 C CNN
F 2 "Resistors:RES_0603" H 4850 4400 60  0001 C CNN
F 3 "" H 4850 4400 60  0001 C CNN
	1    4850 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4650 4850 4700
Wire Wire Line
	5000 4050 4850 4050
Connection ~ 4850 4050
$Comp
L +3.3V #PWR052
U 1 1 56D5BF98
P 5700 4050
F 0 "#PWR052" H 5700 4010 30  0001 C CNN
F 1 "+3.3V" H 5780 4080 30  0000 C CNN
F 2 "" H 5700 4050 60  0001 C CNN
F 3 "" H 5700 4050 60  0001 C CNN
	1    5700 4050
	1    0    0    1   
$EndComp
Wire Wire Line
	5700 4050 5600 4050
$Comp
L GND #PWR045
U 1 1 59AD4F05
P 13250 3300
F 0 "#PWR045" H 13340 3280 30  0001 C CNN
F 1 "GND" H 13250 3220 30  0001 C CNN
F 2 "" H 13250 3300 60  0001 C CNN
F 3 "" H 13250 3300 60  0001 C CNN
	1    13250 3300
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 59AD4F0B
P 13250 3050
F 0 "C11" H 13150 2950 50  0000 L CNN
F 1 "1u" H 13150 3150 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 13350 2900 28  0001 C BNN
F 3 "" H 13250 3050 60  0001 C CNN
	1    13250 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	13250 3250 13250 3300
Wire Wire Line
	13250 2850 13250 2750
Wire Wire Line
	13250 2750 12950 2750
Connection ~ 12950 2750
$Comp
L R R1
U 1 1 59AD5F80
P 5550 6550
F 0 "R1" V 5630 6400 50  0000 C CNN
F 1 "100R" V 5550 6550 50  0000 C CNN
F 2 "Resistors:RES_0603" V 5630 6650 28  0001 C CNN
F 3 "" H 5550 6550 60  0000 C CNN
	1    5550 6550
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 59AD5FF5
P 5550 6950
F 0 "R4" V 5630 6800 50  0000 C CNN
F 1 "100R" V 5550 6950 50  0000 C CNN
F 2 "Resistors:RES_0603" V 5630 7050 28  0001 C CNN
F 3 "" H 5550 6950 60  0000 C CNN
	1    5550 6950
	0    1    1    0   
$EndComp
Text Notes 1000 5600 0    60   ~ 0
Programming & debug
$Comp
L GND #PWR03
U 1 1 59AD39CD
P 1900 6000
F 0 "#PWR03" H 1990 5980 30  0001 C CNN
F 1 "GND" H 1900 5920 30  0001 C CNN
F 2 "" H 1900 6000 60  0001 C CNN
F 3 "" H 1900 6000 60  0001 C CNN
	1    1900 6000
	0    1    1    0   
$EndComp
Text Label 1450 5900 0    60   ~ 0
SWCLK
Text Label 1450 6100 0    60   ~ 0
SWDIO
Text Label 1600 6200 0    60   ~ 0
UART_TX
Text Label 1600 6300 0    60   ~ 0
UART_RX
$Comp
L PwrMCU #PWR02
U 1 1 59AD39D7
P 1800 5800
F 0 "#PWR02" H 1800 5850 30  0001 C CNN
F 1 "PwrMCU" H 1870 5830 30  0000 C CNN
F 2 "" H 1800 5800 60  0000 C CNN
F 3 "" H 1800 5800 60  0000 C CNN
	1    1800 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 6200 1350 6200
Wire Wire Line
	1350 6300 1600 6300
Wire Wire Line
	1350 5800 1800 5800
Wire Wire Line
	1350 6100 1450 6100
Wire Wire Line
	1450 5900 1350 5900
$Comp
L CONN_6 XL1
U 1 1 59AD39E3
P 1150 6050
F 0 "XL1" V 1100 6050 50  0000 C CNN
F 1 "CONN_6" V 1200 6050 50  0000 C CNN
F 2 "Connectors:PLS-6" H 1150 6050 60  0001 C CNN
F 3 "" H 1150 6050 60  0000 C CNN
F 4 "6" V 1150 6050 60  0001 C CNN "SolderPointsDIP"
	1    1150 6050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1350 6000 1900 6000
NoConn ~ 1600 9800
NoConn ~ 1500 9800
NoConn ~ 1400 9800
NoConn ~ 1300 9800
$Comp
L GND #PWR032
U 1 1 59AD55EC
P 7050 4400
F 0 "#PWR032" H 7140 4380 30  0001 C CNN
F 1 "GND" H 7050 4320 30  0001 C CNN
F 2 "" H 7050 4400 60  0001 C CNN
F 3 "" H 7050 4400 60  0001 C CNN
	1    7050 4400
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR036
U 1 1 59AD55F2
P 7250 4400
F 0 "#PWR036" H 7340 4380 30  0001 C CNN
F 1 "GND" H 7250 4320 30  0001 C CNN
F 2 "" H 7250 4400 60  0001 C CNN
F 3 "" H 7250 4400 60  0001 C CNN
	1    7250 4400
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 59AD55F8
P 6850 4400
F 0 "#PWR011" H 6940 4380 30  0001 C CNN
F 1 "GND" H 6850 4320 30  0001 C CNN
F 2 "" H 6850 4400 60  0001 C CNN
F 3 "" H 6850 4400 60  0001 C CNN
	1    6850 4400
	-1   0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 59AD5600
P 6850 3500
F 0 "R6" V 6930 3350 50  0000 C CNN
F 1 "1k" V 6850 3500 50  0000 C CNN
F 2 "Resistors:RES_0603" V 6930 3600 28  0001 C CNN
F 3 "" V 6930 3350 60  0000 C CNN
F 4 "0.5" V 7030 3450 60  0001 C CNN "Price"
F 5 "2" V 7130 3550 60  0001 C CNN "SolderPoints"
	1    6850 3500
	1    0    0    1   
$EndComp
$Comp
L CRYSTAL_H XTAL1
U 1 1 59AD5609
P 7050 3850
F 0 "XTAL1" V 6950 4100 60  0000 C CNN
F 1 "12MHz" V 7050 4100 60  0000 C CNN
F 2 "Quartz:0503x4-4" H 7075 3975 39  0001 C CNN
F 3 "" H 7050 4125 60  0001 C CNN
F 4 "15" H 7150 4225 60  0001 C CNN "Price"
F 5 "4" H 7250 4325 60  0001 C CNN "SolderPoints"
	1    7050 3850
	-1   0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 59AD5612
P 7250 4150
F 0 "C9" V 7150 3950 50  0000 L CNN
F 1 "18pF" V 7150 4250 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 7350 4000 28  0001 C BNN
F 3 "" H 7150 4050 60  0001 C CNN
F 4 "0.5" H 7250 4150 60  0001 C CNN "Price"
F 5 "2" H 7350 4250 60  0001 C CNN "SolderPoints"
	1    7250 4150
	-1   0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 59AD561B
P 6850 4150
F 0 "C1" V 6750 3950 50  0000 L CNN
F 1 "18pF" V 6750 4250 50  0000 L CNN
F 2 "Capacitors:CAP_0603" V 6950 4000 28  0001 C BNN
F 3 "" H 6750 4050 60  0001 C CNN
F 4 "0.5" H 6850 4150 60  0001 C CNN "Price"
F 5 "2" H 6950 4250 60  0001 C CNN "SolderPoints"
	1    6850 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 3850 7250 3850
Wire Wire Line
	6850 3950 6850 3750
Wire Wire Line
	6850 3850 6900 3850
Connection ~ 6850 3850
Connection ~ 7250 3850
Wire Wire Line
	7250 4400 7250 4350
Wire Wire Line
	6850 4350 6850 4400
Wire Wire Line
	7050 4400 7050 4000
Wire Wire Line
	7250 2950 7250 3950
Wire Wire Line
	6450 3050 6850 3050
Wire Wire Line
	6850 3050 6850 3250
Wire Wire Line
	6450 2950 7250 2950
$Comp
L MCP1700 DA2
U 1 1 59AD72E7
P 9200 10100
F 0 "DA2" H 9225 10537 60  0000 C CNN
F 1 "MCP1700" H 9225 10431 60  0000 C CNN
F 2 "SOT:SOT23-3" H 9250 10300 60  0001 C CNN
F 3 "http://datasheet.elcodis.com/pdf2/93/72/937252/mcp1700-1202e.pdf" H 9350 10400 60  0001 C CNN
F 4 "10" H 9450 10500 60  0001 C CNN "Price"
F 5 "3" H 9550 10600 60  0001 C CNN "SolderPoints"
F 6 "" H 9650 10700 60  0001 C CNN "PN"
	1    9200 10100
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR037
U 1 1 59AD764B
P 8650 9950
F 0 "#PWR037" H 8650 9910 30  0001 C CNN
F 1 "+5V" H 8730 9980 30  0000 C CNN
F 2 "~" H 8650 9950 60  0000 C CNN
F 3 "~" H 8650 9950 60  0000 C CNN
	1    8650 9950
	-1   0    0    -1  
$EndComp
$Comp
L +3.3V #PWR044
U 1 1 59AD77DE
P 9800 9950
F 0 "#PWR044" H 9800 9910 30  0001 C CNN
F 1 "+3.3V" H 9880 9980 30  0000 C CNN
F 2 "" H 9800 9950 60  0001 C CNN
F 3 "" H 9800 9950 60  0001 C CNN
	1    9800 9950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 9950 9700 9950
Wire Wire Line
	8750 9950 8650 9950
$Comp
L GND #PWR038
U 1 1 59AD79F1
P 9200 10600
F 0 "#PWR038" H 9290 10580 30  0001 C CNN
F 1 "GND" H 9200 10520 30  0001 C CNN
F 2 "" H 9200 10600 60  0001 C CNN
F 3 "" H 9200 10600 60  0001 C CNN
	1    9200 10600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 10550 9200 10600
$Comp
L Logo Logo1
U 1 1 59AD86FA
P 15150 9400
F 0 "Logo1" H 15428 9453 60  0000 L CNN
F 1 "Logo" H 15428 9347 60  0000 L CNN
F 2 "Pictures:Ostranna_16d9_13d5" H 15150 9400 60  0001 C CNN
F 3 "" H 15150 9400 60  0000 C CNN
	1    15150 9400
	1    0    0    -1  
$EndComp
$Comp
L LED_5050 Q1
U 1 1 59AD9006
P 6400 6750
F 0 "Q1" H 6400 7187 60  0000 C CNN
F 1 "LED_5050" H 6400 7081 60  0000 C CNN
F 2 "LEDs:LED_5050" H 6100 7000 60  0001 C CNN
F 3 "" H 6200 7100 60  0001 C CNN
F 4 "7" H 6300 7200 60  0001 C CNN "Price"
F 5 "6" H 6400 7300 60  0001 C CNN "SolderPoints"
	1    6400 6750
	1    0    0    -1  
$EndComp
$EndSCHEMATC
