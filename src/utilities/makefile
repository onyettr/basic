# Base location defaults
INC_DIR		= 	../../include

# Tools and options
CC  		= 	gcc
LINK  		= 	gcc

#*******************************************************************************
# Build options
#*******************************************************************************

# Main CC and Link build strings
DEBUG		= 	-g
CFLAGS		= 	-c -std=c99 -Wall -pedantic -DTEST_HARNESS $(PFLAGS) -I $(INC_DIR)
LFLAGS		= 	$(PFLAGS) 

#*******************************************************************************
#*******************************************************************************

all:	binarytree.exe 

binarytree.exe:	binarytree.o
	$(LINK) binarytree.o $(LFLAGS) -o binarytree.exe

#*******************************************************************************
# Object builds
#*******************************************************************************

binarytree.o:		 binarytree.c
	$(CC) $(CFLAGS) $(DEBUG) binarytree.c -o binarytree.o

#*******************************************************************************
# remove all libs, objs and intermediates
#*******************************************************************************
clean:
	rm -f binarytree.exe
	rm -f binarytree.o

#
# Fin
#
