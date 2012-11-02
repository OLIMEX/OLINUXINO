CC = gcc
CFLAG = -c

all: lcd

lcd: s1d15g00.o main.o
	$(CC) s1d15g00.o main.o -o lcd
	
main.o: main.c
	$(CC) $(CFLAG) main.c
	
s1d15g00.o: s1d15g00.c
	$(CC) $(CFLAG) s1d15g00.c
	
clean:
	rm -rf *o lcd
