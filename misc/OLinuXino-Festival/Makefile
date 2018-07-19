#-----------------------------------------------------------
#Generic Makefile
#
# Author: Stefan Mavrodiev @ Olimex LTD
# Date	: 2013-08-20
#
# Changelog:
# 2013-08-20	-	Initial release
#
#-----------------------------------------------------------

FES_DIR	=	/root/festival

TARGET	=	Festival
CC		=	g++
CFLAGS	=	-Wall -I$(FES_DIR)/festival/src/include/ -I$(FES_DIR)/speech_tools/include/ -I./inc/
LINKER	=	g++ -o
LFLAGS	=	-L$(FES_DIR)/lib/ -lFestival -L$(FES_DIR)/speech_tools/ -l estools -l estbase -l eststring


SRCDIR	=	src
INCDIR	=	inc
OBJDIR	=	obj
BINDIR	=	bin


SOURCES		:=	$(wildcard $(SRCDIR)/*.cpp)
INCLUDES	:=	$(wildcard $(INCDIR)/*.h)
OBJECTS		:=	$(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm			=	rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS) 
	@echo "Linking complete!"
	
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully"
	
.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"
	
.PHONE: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Exacutable removed!"

