Homework 5
ECE 497
Ben Paras

Beagle Bone memory map is also included in the .doc file

The .png files contain the pictures of the input/output dealy with gpioThru running from an input from a function generator square wave at 10Khz. The delay did increase when another ssh was opened, but it also decreased when 'nice' was used on gpioThru. Open the .png files to see the specific values.

Usage: ./gpioThru

Copies input from GPIO_07 and GPIO20 to USR3 and USR2 respectively by usign mmap() calls


