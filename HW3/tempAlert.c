/*
    Ben Paras	
    HW3 ECE497
    tempAlert.c
    
    Program keeps sensing temperature via TMP101's connected to gpio19 and gpio 20. 
    It sets up the TMP101's via shell script and waits for the ALERT pin on the TMP101's 
    to trigger an interrupt. 
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "i2c-dev.h"
#include <fcntl.h>
#include <signal.h>	// Defines signal-handling functions (i.e. trap Ctrl-C)
#include <poll.h>
#include "gpio-utils.h"

 /****************************************************************
 * Constants
 ****************************************************************/
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64
#define INTPIN1 30
#define INTPIN2 60

/****************************************************************
 * Global variables
 ****************************************************************/
int keepgoing = 1;	// Set to 0 when ctrl-c is pressed

/****************************************************************
 * signal_handler
 ****************************************************************/
void signal_handler(int sig);
// Callback called when SIGINT is sent to the process (Ctrl-C)
void signal_handler(int sig)
{
	printf( "Ctrl-C pressed, cleaning up and exiting..\n" );
	keepgoing = 0; 
}


/****************************************************************
 * Main
 ****************************************************************/

int main(int argc, char *argv[])
{
	struct pollfd fdset[2];
	int nfds = 2;
	unsigned int gpio1, gpio2;
	int gpio_fd1, gpio_fd2;
	char *end;
	int rc;
	char buf[MAX_BUF];

	// no parameters necessary on this one

	// Set the signal callback for Ctrl-C
	signal(SIGINT, signal_handler);

	gpio1 = INTPIN1;
	gpio_export(gpio1);
	gpio_set_dir(gpio1, "in");
	gpio_set_edge(gpio1, "both");
	gpio_fd1 = gpio_fd_open(gpio1, O_RDONLY);

	gpio2= INTPIN2;
	gpio_export(gpio2);
	gpio_set_dir(gpio2, "in");
	gpio_set_edge(gpio2, "both");
	gpio_fd2 = gpio_fd_open(gpio2, O_RDONLY);

	system("sh tempAlertSet.sh");

	while (keepgoing) 
	{
	
		memset((void*)fdset, 0, sizeof(fdset));

		fdset[0].fd = gpio_fd1;
		fdset[0].events = POLLPRI;
		fdset[1].fd = gpio_fd2;
		fdset[1].events = POLLPRI;
		
		
		

		rc = poll(fdset, nfds, POLL_TIMEOUT);      

		if (rc < 0) {
			printf("\n no buttons were pressed :( \n");
			return -1;
		}
      
		if (rc == 0) {
			printf("Waiting for temp change....\n");
		}
		
		

		if (fdset[0].revents & POLLPRI) {
			lseek(fdset[1].fd, 0, SEEK_SET);
			read(fdset[0].fd, buf, 1);
			printf("\n Temperature 1 triggered! \n");
			system("sh tempAlertSense.sh");
		}

		if (fdset[1].revents & POLLPRI) {
			lseek(fdset[1].fd, 0, SEEK_SET);
			read(fdset[1].fd, buf, 1);
			printf("\n Temperature 2 triggered! \n");
			system("sh tempAlertSense.sh");
		}


		fflush(stdout);
	}
	gpio_fd_close(gpio_fd1);
	gpio_fd_close(gpio_fd2);
	return 0;
}
