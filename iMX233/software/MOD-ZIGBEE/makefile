CC = gcc
CFLAG = -c -Wall
EXECUTABLE = mod-zigbee
MODULES = uart

all: $(EXECUTABLE)

$(EXECUTABLE): $(MODULES).o main.o
	$(CC) $(MODULES).o main.o -o $(EXECUTABLE)

main.o: main.c
	$(CC) $(CFLAG) main.c

$(MODULES).o: $(MODULES).c
	$(CC) $(CFLAG) $(MODULES).c
	
clean:
	rm -rf *.o $(EXECUTABLE)
