# I2C tools for Linux
#
# Copyright (C) 2007  Jean Delvare <khali@linux-fr.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

TOOLS_CFLAGS	:= -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		   -W -Wundef -Wmissing-prototypes 

TOOLS_TARGETS	:= my2cset

#
# Programs
#
all:	matrixLEDi2c myi2cget tempAlert

matrixLEDi2c:  matrixLEDi2c.o i2cbusses.o
	$(CC) $(LDFLAGS) -o $@ $^

myi2cget:   myi2cget.o
	$(CC) $(LDFLAGS) -o $@ $^

tempAlert: tempAlert.o gpio-utils.o
	$(CC) $(LDFLAGS) -o $@ $^

etchLED: etchLED.o gpio-utils.o i2cbusses.o
	$(CC) $(LDFLAGS) -o $@ $^

#
# Objects
#

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm matrixLEDi2c matrixLEDi2c.o i2cbusses.o myi2cget myi2cget.o
