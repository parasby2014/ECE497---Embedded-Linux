#!/bin/bash

while true 
do
temp1=`i2cget -y 1 0x49 0`
temp2=`i2cget -y 1 0x4a 0`

t0x49=`echo $((temp1))*1.8+32 | bc`
echo "Sensor 1:" $t0x49 "F" $temp1

t0x4a=`echo $((temp2))*1.8+32 | bc`
echo "Sensor 2:" $t0x4a "F" $temp2

sleep 1
clear
done
