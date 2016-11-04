CFLAGS += $(INCLUDES) -O -Wall -std=gnu99

all: serial_check 

serial_check: serial_check.o cmd.o p_serial.o
	$(CC) $(CFLAGS) -o serial_check serial_check.o cmd.o p_serial.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $*.o


clean:
	rm -f $(OBJ) serial_check *.o *.a
