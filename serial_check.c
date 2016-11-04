/*
 *
 *    a simple program for usage demonstration
 *
 *       does an echo check over serial communication
 *
 */



#include <stdio.h>   /* standard input/output definitions */
#include <stdlib.h>   /* several general purpose functions */
#include <unistd.h>  /* unix standard function definitions */
#include <termios.h> /* posix terminal control definitions */
#include <string.h>  /* string function definitions */

#include "cmd.h"
#include "p_serial.h"



void echo_check(int fd)
{
	const int buf_max = 256;
	int timeout = 1000;
	char eolchar = '%';

	char buf[buf_max];
	memset(buf,0,buf_max);

	serial_write(fd, "hi");
	serial_read_until(fd, buf, eolchar, buf_max, timeout);
	printf("%s\n", buf);
}


int main(int argc, char *argv[])
{
	char *cmd_argv[2];

	if( cmd_args(argc, argv, cmd_argv) == -1 )
	{
		usage();
		return -1;
	}

	const char *port_name = cmd_argv[0];
	int baud = atoi( cmd_argv[1] );

	int fd = init_port(port_name, baud);

	if( fd != -1 )
	{	
		echo_check(fd);

		close_port(fd);
	}

	return 0;
}



