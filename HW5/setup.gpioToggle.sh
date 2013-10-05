#!/bin/bash
# Set up gpio 7 and gpio 20 to read and gpio 3 to write
# modified by Ben Paras 10/4/13
cd /sys/class/gpio
echo 3 > export
echo 7 > export
echo 20 > export
echo in  > gpio7/direction
echo in  > gpio20/direction
echo out > gpio3/direction
