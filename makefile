#*******************************************************************************
# File name        : makefile
# File description : basic interpreter
# Author           : ronyett
#*******************************************************************************

SRC_DIR		= 	.
OBJECT_DIR	= 	$(SRC_DIR)/object
MAKE_DIR_CMD	= 	mkdir $(OBJECT_DIR)

CC  		= 	gcc
LINK  		= 	gcc
AR		= 	ar
CHK   		= 	checkmk
CHECK_FOR_CHK	:= 	$(shell command -v $(CHK) 2> /dev/null)

#*******************************************************************************
# Build options
#*******************************************************************************

# gcov and gprof build options
COVPFLAGS	= 	-fprofile-arcs -ftest-coverage
PROFLAGS	= 	-pg
#PFLAGS		= 	$(COVFLAGS)

# Main CC and Link build strings
DEBUG		= 	-g
CFLAGS		= 	-c -std=c99 -Wall -pedantic $(PFLAGS)
LFLAGS		= 	$(PFLAGS) -static -L.

# -DDEBUG_TRACE	Will turn on deep trace per function

#
# Code checking with splint
#
CODE_CHECK       = 	splint
CODE_CHECK_ARGS	 = 	-showfunc -mustfreefresh -nullpass -nullret -noeffect

#
# Libs, objs targets
# libstack library is built from trap handling and the stack implementation. 
#
OBJS  		     = $(OBJECT_DIR)/main.o 
LIBS  		     = liblister.a
TEST_STACK 	     = 

#*******************************************************************************
# Build targets:
# all		Creates object directory, builds executable and runs checker
# lib		Build only the list library, no test harness
# splint-it	run the Syntax checker
# clean		Delete object and library files
#*******************************************************************************

all:	$(OBJECT_DIR) basic.exe liblister.a test_harness

lib:	$(LIBS)

basic.exe:	$(OBJS) $(LIBS)
	$(LINK) $(OBJS) $(LFLAGS) -llister -o basic.exe

liblister.a:	lister.o 
	$(AR) rcs liblister.a lister.o

$(OBJECT_DIR):
	-$(MAKE_DIR_CMD)

$(OBJECT_DIR)/main.o:		main.c
	$(CC) $(CFLAGS) $(DEBUG) main.c -o $(OBJECT_DIR)/main.o
$(OBJECT_DIR)/lister.o:		lister.c
	$(CC) $(CFLAGS) $(DEBUG) lister.c -o $(OBJECT_DIR)/lister.o

#
# This is the "check" target: Test harness is in stack_check.ts file and 
# this is converted by "check" into a C file which is linked to give another
# executable. 
# 
# NOTE: This will not build if you have the Profiling enabled as the libstack.a 
# contains gcov 
#

test_harness:
	@echo "** test harness TODO **"

#test_harness: libstack.a stack_check.ts
#ifndef CHECK_FOR_CHK
#	@echo "** checkmk command not found"
#else
#	$(CHK) stack_check.ts > stack_check.c
#	$(CC) -o stack_check.exe stack_check.c -static -L. -lcheck -lstack 
#endif

#
# Code checking target
#
splint-it:
	$(CODE_CHECK) $(CODE_CHECK_ARGS) lister.c   
	$(CODE_CHECK) $(CODE_CHECK_ARGS) main.c


clean:
	rm -f basic.exe
	rm -f $(OBJECT_DIR)/lister.o
	rm -f liblister.a
	rm -f $(OBJECT_DIR)/main.o
	rm -f *.gcno
	rm -f *.gcda
	rm -f gmon.out
	rm -f core

#
# Fin
#

