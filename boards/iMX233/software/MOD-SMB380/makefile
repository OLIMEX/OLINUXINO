CC = gcc
CFLAG = -c -Wall
EXECUTABLE = mod-smb380
MAIN = main

all: $(EXECUTABLE)

$(EXECUTABLE): spi.o i2c.o $(MAIN).o
	$(CC) spi.o i2c.o $(MAIN).o -o $(EXECUTABLE)

$(MAIN).o: $(MAIN).c
	$(CC) $(CFLAG) $(MAIN).c

spi.o: spi.c
	$(CC) $(CFLAG) spi.c
	
i2c.o: i2c.c
	$(CC) $(CFLAG) i2c.c
	
clean:
	rm -rf *.o $(EXECUTABLE)
