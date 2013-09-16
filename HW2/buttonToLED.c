/* 
Ben Paras
ECE497
HW2 

Usage: ./buttonToLED <b1> <b2> <b3> <b4>

4 buttons that light up 4 different LEDs
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>	// Defines signal-handling functions (i.e. trap Ctrl-C)
#include "gpio-utils.h"

 /****************************************************************
 * Constants
 ****************************************************************/
 
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64
#define GPIO_OUT1 60
#define GPIO_OUT2 31
#define GPIO_OUT3 50
#define GPIO_OUT4 48

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
int main(int argc, char **argv, char **envp)
{
	struct pollfd fdset[5];
	int nfds = 5;
	int toggle1 = 0; // 27/115
	int toggle2 = 0; // 30/112
	int toggle3 = 0; // 41/20
	int toggle4 = 0; // 42/7
	int gpio_fd1, gpio_fd2, gpio_fd3, gpio_fd4, timeout, rc;
	int gpio_fd1_out, gpio_fd2_out, gpio_fd3_out, gpio_fd4_out;
	char buf[MAX_BUF];
	unsigned int gpio1, gpio2, gpio3, gpio4;
	int len;

	if (argc < 5) {
		printf("Usage: buttonToLED <gpio-pin1> <gpio-pin2> <gpio-pin3> <gpio-pin4>\n\n");
		printf("Corresponding button will light corresponding LED");
		exit(-1);
	}

	// Set the signal callback for Ctrl-C
	signal(SIGINT, signal_handler);

	gpio1 = atoi(argv[1]);
	gpio2 = atoi(argv[2]);
	gpio3 = atoi(argv[3]);
	gpio4 = atoi(argv[4]);
	
	gpio_export(gpio1);
	gpio_export(gpio2);
	gpio_export(gpio3);
	gpio_export(gpio4);
	gpio_export(GPIO_OUT1);
	gpio_export(GPIO_OUT2);
	gpio_export(GPIO_OUT3);
	gpio_export(GPIO_OUT4);

	gpio_set_dir(gpio1, "in");
	gpio_set_dir(gpio2, "in");
	gpio_set_dir(gpio3, "in");
	gpio_set_dir(gpio4, "in");
	gpio_set_dir(GPIO_OUT1, "out");
	gpio_set_dir(GPIO_OUT2, "out");
	gpio_set_dir(GPIO_OUT3, "out");
	gpio_set_dir(GPIO_OUT4, "out");

	gpio_set_edge(gpio1, "rising");  // Can be rising, falling or both
	gpio_set_edge(gpio2, "rising");
	gpio_set_edge(gpio3, "rising");
	gpio_set_edge(gpio4, "rising");

	gpio_fd1 = gpio_fd_open(gpio1, O_RDONLY);
	gpio_fd2 = gpio_fd_open(gpio2, O_RDONLY);
	gpio_fd3 = gpio_fd_open(gpio3, O_RDONLY);
	gpio_fd4 = gpio_fd_open(gpio4, O_RDONLY);
	gpio_fd1_out = gpio_fd_open(GPIO_OUT1, O_RDONLY);
	gpio_fd2_out = gpio_fd_open(GPIO_OUT2, O_RDONLY);
	gpio_fd3_out = gpio_fd_open(GPIO_OUT3, O_RDONLY);
	gpio_fd4_out = gpio_fd_open(GPIO_OUT4, O_RDONLY);

	timeout = POLL_TIMEOUT;
 
	while (keepgoing) {
		memset((void*)fdset, 0, sizeof(fdset));

		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;
      
		fdset[1].fd = gpio_fd1;
		fdset[1].events = POLLPRI;
		fdset[2].fd = gpio_fd2;
		fdset[2].events = POLLPRI;
		fdset[3].fd = gpio_fd3;
		fdset[3].events = POLLPRI;
		fdset[4].fd = gpio_fd4;
		fdset[4].events = POLLPRI;

		rc = poll(fdset, nfds, timeout);      

		if (rc < 0) {
			printf("\n no buttons were pressed :( \n");
			return -1;
		}
      
		if (rc == 0) {
			printf("press a button! ");
		}
            
		if (fdset[1].revents & POLLPRI) {
			lseek(fdset[1].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[1].fd, buf, MAX_BUF);
			toggle1 = !toggle1;
			gpio_set_value(GPIO_OUT1, toggle1);
			printf("\n BUTTON1 PRESSED\n");
		}

		if (fdset[2].revents & POLLPRI) {
			lseek(fdset[2].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[2].fd, buf, MAX_BUF);
			toggle2 = !toggle2;
			gpio_set_value(GPIO_OUT2, toggle2);
			printf("\n BUTTON2 PRESSED\n");
		}

		if (fdset[3].revents & POLLPRI) {
			lseek(fdset[3].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[3].fd, buf, MAX_BUF);
			toggle3 = !toggle3;
			gpio_set_value(GPIO_OUT3, toggle3);
			printf("\n BUTTON3 PRESSED\n");
		}

		if (fdset[4].revents & POLLPRI) {
			lseek(fdset[4].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[4].fd, buf, MAX_BUF);
			toggle4 = !toggle4;
			gpio_set_value(GPIO_OUT4, toggle4);
			printf("\n BUTTON4 PRESSED\n");
		}

		if (fdset[0].revents & POLLIN) {
			(void)read(fdset[0].fd, buf, 1);
			printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
		}

		fflush(stdout);
	}
	gpio_fd_close(gpio_fd1);
	gpio_fd_close(gpio_fd2);
	gpio_fd_close(gpio_fd3);
	gpio_fd_close(gpio_fd4);
	gpio_fd_close(gpio_fd1_out);
	gpio_fd_close(gpio_fd2_out);
	gpio_fd_close(gpio_fd3_out);
	gpio_fd_close(gpio_fd4_out);

	return 0;
}

