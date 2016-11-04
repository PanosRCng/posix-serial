/*
 *
 *  some command line utilities for the program
 *
 */



#include <stdio.h>   /* Standard input/output definitions */
#include <getopt.h>  /* UNIX parse arguments */
#include <string.h>  /* String function definitions */
#include <ctype.h>

#include "cmd.h"


void usage(void)
{
    printf("\nUsage: serial_check -b <baud_rate> -p <port_name> [OPTIONS]\n"
    "\n"
    "Options:\n"
    "  -h, --help                 Print this help message\n"
    "  -b, --baud=baudrate        Baudrate (bps) of connection (default 9600)\n"
    "  -p, --port=port_name       The serial port\n"
    "\n"
    "Example: serial_check -p /dev/ttyUSB0 -b 9600 \n\n");
}


int cmd_args(int argc, char *argv[], char *cmd_argv[])
{
	if( !check_len(argc, argv) )
	{
		return -1;
	}

	int cmd_argc = 0;
	int c;

	static struct option long_options[] = {
        	{"help",  no_argument,       0, 'h'},
        	{"port",  required_argument, 0, 'p'},
        	{"baud",  required_argument, 0, 'b'},
        	{0,       0,                 0, 0}
    	};

	while(1)
	{
		// getopt_long stores the option index here
		int option_index = 0;

		c = getopt_long(argc, argv, "hp:b:", long_options, &option_index);

		// detect the end of the options
		if(c == -1)
			break;

		switch(c)
        	{
        		case 0:
				break;
			case 'p':
				cmd_argv[0] = optarg;
				cmd_argc++;
				break;
			case 'b':
				cmd_argv[1] = optarg;
				cmd_argc++;
				break;
        	}
	}


	if( !isint(cmd_argv[1]) )
	{
		return -1;
	}

	return (cmd_argc == 2) ? 0 : -1;
}


int check_len(int argc, char *argv[])
{
	for(int i=0; i<argc; i++)
	{
		if( strlen(argv[i]) > 20 )
		{
			return 0;
		}
	}

	return 1;
}


int isint(char *str)
{
	for(int i=0; i<strlen(str); i++)
	{
		if( !isdigit(str[i]) )
		{
			return 0;
		}
	}

	return 1;
}


