
Configuration Details:

I found CONFIG_CPU_V6, CONFIG_CPU_V6K, and CONFIG_CPU_V7:


obj-$(CONFIG_CPU_V6)		+= proc-v6.o
obj-$(CONFIG_CPU_V6K)		+= proc-v6.o
obj-$(CONFIG_CPU_V7)		+= proc-v7.o

from /home/bp/exercises/linux-dev/KERNEL/arch/arm/mm/Makefile

I also found CONFIG_CPU_V7 = y from .config as well. I couldin't find the other two for in the configuration file.

b start_kernel is located on line 389 in /arch/arm/kernel/head.S:

ENTRY(__secondary_switched)
	ldr	sp, [r7, #4]			@ get secondary_data.stack
	mov	fp, #0
	b	secondary_start_kernel

this branches to main.c which is in the path /init/main.c


systemd: 

boneServer.service 

