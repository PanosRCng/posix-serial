#ifndef p_serial_h
#define P_serial_h



// inits the serial port
int init_port(const char *port_name, int baud);


// opens the serial port
int open_port(const char *port_name);


// closes the serial port
void close_port(int fd);


// returns the baud rate code word, for a given baud rate speed
int baud(int baud_rate);


// sets some options about the connection
void set_options(struct termios *options);


// writes a string to port
int serial_write(int fd, const char *buf);


// read from port until get eol character, or timeout, or the buffer is full
int serial_read_until(int fd, char* buf, char eol, int buf_max, int timeout);


// flush port
int flush_port(int fd);


#endif
