#!/bin/bash

t0x49=`i2cget -y 1 0x49 0`
t0x4a=`i2cget -y 1 0x4a 0`

t0x49=`echo $((t0x49))*1.8+32 | bc`
echo "Sensor 1:" $t0x49 "F"

t0x4a=`echo $((t0x4a))*1.8+32 | bc`
echo "Sensor 2:" $t0x4a "F"

exit
