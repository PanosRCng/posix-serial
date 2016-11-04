/*
 *    p_serial
 *
 *
 *	- posix serial port communication utilities
 *
 *  	- based on:
 *		    Serial Programming Guide for POSIX Operating Systems
 *	   	    (https://www.cmrr.umn.edu/~strupp/serial.html#CONTENTS) 
 *
 *
 *	- developed (and tested) mainly for communication 
 *        with arduino and other microcontrollers
 */


#include <stdio.h>   /* Standard input/output definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <string.h>  /* string function definitions */

#include "p_serial.h"



int init_port(const char *port_name, int baud_rate)
{
	int fd = open_port(port_name);

	if(fd == -1)
	{
		return -1;
	}

	struct termios options;

	//get the current options for the port
   	if (tcgetattr(fd, &options) < 0)
	{
        	perror("init_port: could not get current port options");
        	return -1;
    	}

	speed_t brate = baud(baud_rate);

	// set the baud rate in the termios structure
	cfsetispeed(&options, brate);
	cfsetospeed(&options, brate);

	set_options(&options);

	// set the new options for the port
	// all changes should occur immediately
	if( tcsetattr(fd, TCSAFLUSH, &options) < 0)
	{
	        perror("init_port: could not set port options");
	        return -1;
	}

	flush_port(fd);

	return fd;
}


int open_port(const char *port_name)
{
	// the file descriptor for the port
	int fd;

	// O_NOCTTY doesn't want to be the "controlling terminal" for that port
	// O_NDELAY doesn't care what state the DCD signal line is in
	fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);
	
	if (fd == -1)
	{
		perror("open_port: unable to open the serial port");
	}
	else
	{
    		fcntl(fd, F_SETFL, 0);
	}

	return fd;
}


void close_port(int fd)
{
	close(fd);
}


int baud(int baud_rate)
{
	int baud = 9600;

	switch(baud_rate)
	{
    		case 4800:   baud=B4800;   break;
    		case 9600:   baud=B9600;   break;
	    	case 19200:  baud=B19200;  break;
	    	case 38400:  baud=B38400;  break;
    		case 57600:  baud=B57600;  break;
    		case 115200: baud=B115200; break;
    	}

	return baud;
}

void set_options(struct termios *options)
{
	// set 8 bits as the character size, no parity (8N1)
	options->c_cflag &= ~PARENB;
	options->c_cflag &= ~CSTOPB;
	options->c_cflag &= ~CSIZE;
	options->c_cflag |= CS8;

	// no flow control
	options->c_cflag &= ~CRTSCTS;

	// enable the receiver and set local mode
	// CLOCAL ensure that your program does not become the 'owner' of the port
	// CREAD ensure thatthe serial interface driver will read incoming data bytes
	options->c_cflag |= CREAD | CLOCAL;

	// disable software flow control
	options->c_iflag &= ~(IXON | IXOFF | IXANY);

	// set raw input / output
	options->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options->c_oflag &= ~OPOST;

	options->c_cc[VMIN]  = 0;
	options->c_cc[VTIME] = 0;
}


int serial_write(int fd, const char *str)
{
	int len = strlen(str);

	int n = write(fd, str, len);

	if( n!=len )
	{
        	perror("serial: could not write whole string\n");
        	return -1;
    	}

	return 0;
}



int serial_read_until(int fd, char* buf, char eol, int buf_max, int timeout)
{
	char b[1];
	int i=0;

	do
	{ 
		int n = read(fd, b, 1);

		// could not read
        	if( n==-1) return -1;

        	if( n==0 )
		{
			// wait 1 msec try again
        		usleep( 1 * 1000 );
        		timeout--;
        		if( timeout==0 ) return -2;
        		continue;
        	}

        	buf[i] = b[0]; 
        	i++;
    	}
	while( (b[0] != eol) && (i < buf_max && timeout>0) );

	// terminate the string
	buf[i] = 0;
	return 0;
}


int flush_port(int fd)
{
    sleep(2);
    return tcflush(fd, TCIOFLUSH);
}















