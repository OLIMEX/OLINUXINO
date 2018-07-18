CC = gcc
CFLAG = -c -Wall
EXECUTABLE = mod-bt
MAIN = MOD-BT
MODULES = uart

all: $(EXECUTABLE)

$(EXECUTABLE): $(MODULES).o $(MAIN).o
	$(CC) $(MODULES).o $(MAIN).o -o $(EXECUTABLE)

$(MAIN).o: $(MAIN).c
	$(CC) $(CFLAG) $(MAIN).c

$(MODULES).o: $(MODULES).c
	$(CC) $(CFLAG) $(MODULES).c
	
clean:
	rm -rf *.o $(EXECUTABLE)
