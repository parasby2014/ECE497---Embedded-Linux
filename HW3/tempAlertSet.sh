#!/bin/bash

echo 'i2cset -y 1 0x49 2 0x1a'
echo 'i2cset -y 1 0x49 3 0x1b'
echo 'i2cset -y 1 0x4a 2 0x18'
echo 'i2cset -y 1 0x4a 3 0x1a'

#echo 'i2cset -y 1 0x49 2 0x15'
#echo 'i2cset -y 1 0x49 3 0x19'
#echo 'i2cset -y 1 0x4a 2 0x15'
#echo 'i2cset -y 1 0x4a 3 0x19'

exit
