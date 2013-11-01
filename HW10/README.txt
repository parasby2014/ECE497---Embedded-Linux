shrinking:

My default busybox file is 644K. After reconfiguring by removing all editors, it is 600K. With the default configuration and only without 'ls', busybox is 640K which makes the 'ls' command 4K. My busy box had 'httpd' on my default so I removed it to see how much was left. With the default configuration with only 'httpd' removed, my busybox is 636K.

Static Linking:

I configured busybox to 'Build shared libbusybox'. With the default configurations and statically linking, busybox's size went up to 692K. I didn't have to change anything for this to work. 

Running a Web Server:

My busybox, as said before, defaulted with 'httpd' on. It takes about 8K of space since it was 636K without it and 644K with it.

Busybox defaults its webpage from port81 to wherever you start the program up and looks up index.html. I edited index.html and left it in the home directory. 
