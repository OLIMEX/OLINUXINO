CC = gcc
CFLAG = -c -Wall
EXECUTE = MOD-RGB

all: $(EXECUTE)

$(EXECUTE): i2c.o RGB.o
	$(CC) i2c.o RGB.o -o $(EXECUTE)
	
main.o: RGB.c
	$(CC) $(CFLAG) RGB.c
	
i2c.o: i2c.c
	$(CC) $(CFLAG) i2c.c
	
clean:
	rm -rf *.o $(EXECUTE)
