CC = arm-none-linux-gnueabi-gcc
CPP = arm-none-linux-gnueabi-g++
STRIP = arm-none-linux-gnueabi-strip
AR = arm-none-linux-gnueabi-ar

SRCDIRS     =./ \
			  ./render \
        ./decode \
        ./include \
        ./capture
INCLUDES := $(foreach dir,$(SRCDIRS),-I$(dir))
SRCCS    = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJ  	 := $(SRCCS:%.c=%.o)


			  
CFLAGS = -Wall -O0 -g
CFLAGS := $(CFLAGS) $(INCLUDES)

CFLAGS += -D__OS_LINUX
LIBS += -lpthread ./linux_lib/libcedarv_osal.a ./linux_lib/libcedarxalloc.a ./linux_lib/libh264enc.a ./linux_lib/libcedarv.a

TARGET = enc_dec_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CPP) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJ)

