/* 
Ben Paras
ECE497
HW3

etch-a-sketch on LED matrix

Usage: ./etchASketchProg <boardSize> <gpio-pin1> <gpio-pin2> <gpio-pin3> <gpio-pin4> (115, 112, 20, 7)

Press the pushbuttons to etch a sketch. User picks board size. Board clears in 10 seconds if nothing is pressed.

 
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
#include "i2c-dev.h"
#include "i2cbusses.h"

 /****************************************************************
 * Constants
 ****************************************************************/
 
#define POLL_TIMEOUT (10 * 1000) /*  10 seconds */
#define MAX_BUF 64

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
 * Matrix LED functions
 ****************************************************************/

static void help(void) __attribute__ ((noreturn));

static void help(void) {
	fprintf(stderr, "Usage: matrixLEDi2c (hardwired to bus 3, address 0x70)\n");
	exit(1);
}

static int check_funcs(int file) {
	unsigned long funcs;

	/* check adapter functionality */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		fprintf(stderr, "Error: Could not get the adapter "
			"functionality matrix: %s\n", strerror(errno));
		return -1;
	}

	if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
		fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
		return -1;
	}
	return 0;
}

// Writes block of data to the display
static int write_block(int file, __u16 *data) {
	int res;
#ifdef BICOLOR
	res = i2c_smbus_write_i2c_block_data(file, 0x00, 16, 
		(__u8 *)data);
	return res;
#else
/*
 * For some reason the single color display is rotated one column, 
 * so pre-unrotate the data.
 */
	int i;
	__u16 block[I2C_SMBUS_BLOCK_MAX];
//	printf("rotating\n");
	for(i=0; i<8; i++) {
		block[i] = (data[i]&0xfe) >> 1 | 
			   (data[i]&0x01) << 7;
	}
	res = i2c_smbus_write_i2c_block_data(file, 0x00, 16, 
		(__u8 *)block);
	return res;
#endif
}

/****************************************************************
 * Helper Functions
 ****************************************************************/
//initialize board with _
void setUpBoard(int size, char board[size][size])
{		
	int i=0; 
	int j=0;
	printf("\n");
	for(i=0; i < size; i++)
	{
		for(j=0; j < size; j++)
		{
			board[i][j] = '_';
			printf("%c", board[i][j]);
		}
		
		printf("\n");
	}
}

// update board
void updateBoard(int size, char board[size][size], int x, int y)
{
	int i=0; 
	int j=0;
	system("clear");
	printf("Updating...(%d,%d).\n",x ,y);
	board[y][x] = 'x'; 
	for(i=0; i < size; i++)
	{
		for(j=0; j < size; j++)
		{
			printf("%c", board[i][j]);
		}	
		printf("\n");
	}

}

void lightUpBoard(int file, int size, char board[size][size])
{
	int i = 0;
	int j = 0;
	__u16 LED[]=
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	for(i = 0; i<size; i++)
	{
		for(j = 0; j<size; j++)
		{
		 	if(board[i][j] == 'x')
			{	
				LED[(size-i-1)] |= (0x01 << (size-j-1));				
			}
		}
	}
	write_block(file, LED);


}
/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp)
{
	struct pollfd fdset[5];
	int nfds = 5;

	// Check for proper amount of arguments
	if (argc < 5) {
		printf("Usage: etchASketchProg <boardSize> <gpio-pin1> <gpio-pin2> <gpio-pin3> <gpio-pin4>\n\n");
		exit(-1);
	}

	// Initialize vairables
	int boardSize = atoi(argv[1]);
	char buf[MAX_BUF];
	char gameBoard[boardSize][boardSize];
	int x = 0;
	int y = 0;
	int len, timeout, rc;
	int gpio_fd1, gpio_fd2, gpio_fd3, gpio_fd4;
	unsigned int gpio1, gpio2, gpio3, gpio4;
	
	// Set the signal callback for Ctrl-C
	signal(SIGINT, signal_handler);

	timeout = POLL_TIMEOUT;

	
	// initialize GPIO (buttons are wired to gpio 115,112, 20, and 7)
	gpio1 = atoi(argv[2]);
	gpio2 = atoi(argv[3]);
	gpio3 = atoi(argv[4]);
	gpio4 = atoi(argv[5]);
	
	gpio_export(gpio1);
	gpio_export(gpio2);
	gpio_export(gpio3);
	gpio_export(gpio4);

	gpio_set_dir(gpio1, "in");
	gpio_set_dir(gpio2, "in");
	gpio_set_dir(gpio3, "in");
	gpio_set_dir(gpio4, "in");

	gpio_set_edge(gpio1, "rising");  // Can be rising, falling or both
	gpio_set_edge(gpio2, "rising");
	gpio_set_edge(gpio3, "rising");
	gpio_set_edge(gpio4, "rising");

	gpio_fd1 = gpio_fd_open(gpio1, O_RDONLY);
	gpio_fd2 = gpio_fd_open(gpio2, O_RDONLY);
	gpio_fd3 = gpio_fd_open(gpio3, O_RDONLY);
	gpio_fd4 = gpio_fd_open(gpio4, O_RDONLY);

	// Set up LED
	int res, i2cbus, address, file;
	char filename[20];
	int force = 0;

	i2cbus = lookup_i2c_bus("1");
	printf("i2cbus = %d\n", i2cbus);
	if (i2cbus < 0)
		help();

	address = parse_i2c_address("0x70");
	printf("address = 0x%2x\n", address);
	if (address < 0)
		help();

	file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
//	printf("file = %d\n", file);
	if (file < 0
	 || check_funcs(file)
	 || set_slave_addr(file, address, force))
		exit(1);

	// Check the return value on these if there is trouble
	i2c_smbus_write_byte(file, 0x21); // Start oscillator (p10)
	i2c_smbus_write_byte(file, 0x81); // Disp on, blink off (p11)
	i2c_smbus_write_byte(file, 0xe7); // Full brightness (page 15)


	// Initialize game
	printf("Creating board...\n");
	setUpBoard(boardSize, gameBoard);
	printf("ready! board will clear if none are pressed in 10 sec....\n");

	
	
 
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
      
		// If interrupt times out
		if (rc == 0) {
			x = 0;
			y = 0;
			setUpBoard(boardSize, gameBoard);
			printf("Cleared board!\n");
			
		}
            

		// Now check for the 4 buttons via interrupt....
		if (fdset[1].revents & POLLPRI) {
			lseek(fdset[1].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[1].fd, buf, MAX_BUF);
			printf("\n Left PRESSED\n");

			if( x > 0)
			{
				x--;
				updateBoard(boardSize, gameBoard, x, y ); 
			}
			else
			{
				printf("Can't move further, try another direction\n");
				updateBoard(boardSize, gameBoard, x, y ); 
			}
			
		}

		if (fdset[2].revents & POLLPRI) {
			lseek(fdset[2].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[2].fd, buf, MAX_BUF);
			printf("\n Up PRESSED\n");

			if( y > 0)
			{
				y--;
				updateBoard(boardSize, gameBoard, x, y );
			}
			else
			{
				printf("Can't move further, try another direction\n");
				updateBoard(boardSize, gameBoard, x, y ); 
			}
		}

		if (fdset[3].revents & POLLPRI) {
			lseek(fdset[3].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[3].fd, buf, MAX_BUF);
			printf("\n Down PRESSED\n");
			
			if( y < ( boardSize -1)	)
			{
				y++;
				updateBoard(boardSize, gameBoard, x, y );
			}
			else
			{
				printf("Can't move further, try another direction\n");
				updateBoard(boardSize, gameBoard, x, y ); 
			}
		
		}

		if (fdset[4].revents & POLLPRI) {
			lseek(fdset[4].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[4].fd, buf, MAX_BUF);
			printf("\n Right PRESSED\n");
			
			if( x < (boardSize-1))
			{
				x++;
				updateBoard(boardSize, gameBoard, x, y ); 
			}
			else
			{
				printf("Can't move further, try another direction\n");
				updateBoard(boardSize, gameBoard, x, y ); 
			}

		}

		if (fdset[0].revents & POLLIN) {
			(void)read(fdset[0].fd, buf, 1);
			printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
		}


		fflush(stdout);

		lightUpBoard( file, boardSize, gameBoard);
		
	}
	gpio_fd_close(gpio_fd1);
	gpio_fd_close(gpio_fd2);
	gpio_fd_close(gpio_fd3);
	gpio_fd_close(gpio_fd4);
	
	return 0;
}

