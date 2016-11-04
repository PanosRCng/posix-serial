#ifndef cmd_h
#define cmd_h



// shows usage message
void usage(void);


// gets options and arguments from the command line
int cmd_args(int argc, char *argv[], char *cmd_argv[]);


// checks command line arguments length
int check_len(int cmd_argc, char *cmd_argv[]);


// checks if str is the string of an integer
int isint(char *str);


#endif
