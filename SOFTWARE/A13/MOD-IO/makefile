CC = gcc
CFLAG = -c -Wall

all: mod-io

mod-io: i2c.o main.o
	$(CC) i2c.o main.o -o mod-io

main.o: main.c
	$(CC) $(CFLAG) main.c

i2c.o: i2c.c
	$(CC) $(CFLAG) i2c.c
	
clean:
	rm -rf *.o mod-io
