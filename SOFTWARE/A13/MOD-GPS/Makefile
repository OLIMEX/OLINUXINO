CC = gcc
CFLAG = -c -Wall
EXECUTE = MOD-GPS
NAME = GPS
MODULE = uart

all: $(EXECUTE)

$(EXECUTE): $(NAME).o $(MODULE).o
	$(CC) $(MODULE).o $(NAME).o -o $(EXECUTE) -lncurses
	
main.o: $(NAME).c
	$(CC) $(CFLAG) $(NAME).c 

$(MODULE).0: $(MODULE).c
	$(CC) $(CFLAG) $(MODULE).c
	
clean:
	rm -rf *.o $(EXECUTE)
