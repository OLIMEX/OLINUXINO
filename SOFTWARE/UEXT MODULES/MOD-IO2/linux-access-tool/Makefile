CC = gcc
CFLAG = -c -Wall
EXECUTE = modio2tool
NAME = IO2-TOOL

all: $(EXECUTE)

$(EXECUTE): i2c.o $(NAME).o
	$(CC) i2c.o $(NAME).o -o $(EXECUTE)
	
main.o: $(NAME).c
	$(CC) $(CFLAG) $(NAME).c
	
i2c.o: i2c.c
	$(CC) $(CFLAG) i2c.c
	
clean:
	rm -rf *.o $(EXECUTE)
