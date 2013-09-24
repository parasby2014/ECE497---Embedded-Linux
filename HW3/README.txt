Ben Paras
HW3 ECE497

Part 1: 

./tempAlert 

Waits for an interrupt on the Alert pin ont he TMP101's wired to gpio30 and gpio60. When alerted, and interrupt will occur and it will print out the temperature in F. The high and low thresholds are    calibrated according to the room I was in when I tested it so it may not work with other areas. I do not have it to be adjustable at by user input at the moment. The program uses shell scripts to make things more simple.

Due to the inconsisten nature of the temperature sensors, I did no use it in the etchLED program for a combined demo. 

Part 2:

./etchLED <size> <gpio_pin> <gpio_pin> <gpio_pin> <gpio_pin> (I used 8 115 112 20 7) 

Etch-a-sketch program interfaced with the LED. The board wraps around one column when you reach the end on the right side. The program will also print the current board on the terminal. This is much like the program in HW2, but with added LED matrix interface. The board clears every 10 seconds when no button is pressed. 

I did overlay the pins 11,12,13, and 14 as requestsed in the homework assignment with BP-gpio-set.dts, but I did not use all those pins for the etchLED program or the tempAlert program. I did verify their overlay though. This is the print out when I check: 

root@beaglebone:~/exercises/gpio# ./findGPIO.js P9_11 P9_12 P9_13 P9_14
P9_12 (gpio 60) mode: 7 (gpio1_28) 0x078 pullup Receiver Active
P9_11 (gpio 30) mode: 7 (gpio0_30) 0x070 pullup Receiver Active
P9_13 (gpio 31) mode: 7 (gpio0_31) 0x074 pullup Receiver Active
P9_14 (gpio 50) mode: 7 (gpio1_18) 0x048 pullup Receiver Active

