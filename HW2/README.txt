Ben Paras ECE497

Etch-A-Sketch Board game displays in the terminal. 

The four pushbuttons move the position. The board clears in 10 seconds if no buttons are pressed

Usage: ./etchASketchProg <boardSize> <pb1> <pb2> <pb3> <pb4> 

The buttons used on my board are wired up to 115, 112, 20, and 7 gpio pins 


Also buttonsToLED is the simple program that read the swithces and lights a corresponding LED. 

Usage ./buttonsToLED <pb1> <pb2> <pb3> <pb4>

I used 115, 112, 20, and 7. The ouputs are on pins 60,31,50 and 48. 
