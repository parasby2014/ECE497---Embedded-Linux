
Configuration Details:

I can't seem to find CONFIG_CPU_V6 or CONFIG_CPU_V6K so they may be undefined.

CONFIR_CPU_V7 = y from .config

b start_kernel is located on line 389 in /arch/arm/kernel/head.S:

ENTRY(__secondary_switched)
	ldr	sp, [r7, #4]			@ get secondary_data.stack
	mov	fp, #0
	b	secondary_start_kernel

this branches to main.c which is in the path /init/main.c


systemd: 

boneServer.service 

