CC = gcc
CFLAG = -c -Wall
EXECUTE = mod-rs485-iso

all: $(EXECUTE)

$(EXECUTE): i2c.o main.o
	$(CC) i2c.o main.o -o $(EXECUTE)

main.o: main.c
	$(CC) $(CFLAG) main.c

i2c.o: i2c.c
	$(CC) $(CFLAG) i2c.c
	
clean:
	rm -rf *.o $(EXECUTE)
