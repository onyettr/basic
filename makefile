#*******************************************************************************
# File name        : makefile
# File description : basic interpreter
# Author           : ronyett
#*******************************************************************************

# Base location defaults
SRC_DIR		= 	.
OBJECT_DIR	= 	$(SRC_DIR)/object
LIB_DIR		= 	$(SRC_DIR)/libraries
MAKE_DIR_CMD	= 	mkdir $(OBJECT_DIR)
MAKE_LIB_CMD	= 	mkdir $(LIB_DIR)

# Tools and options
CC  		= 	gcc
LINK  		= 	gcc
AR		= 	ar
CHK_TOOL	= 	checkmk
#CODE_CHECK       = 	splint
CODE_CHECK	= 	cppcheck
CHECK_FOR_CHK	:= 	$(shell command -v $(CHK_TOOL) 2> /dev/null)
CHECK_FOR_CPP	:=	$(shell command -v $(CODE_CHECK) 2> /dev/null)

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
LFLAGS		= 	$(PFLAGS) -static -L. -L./$(LIB_DIR)

# -DDEBUG_TRACE	Will turn on deep trace per function

#
# Code checking with splint or cppcheck
#
#CODE_CHECK_ARGS	 = 	-showfunc -mustfreefresh -nullpass -nullret -noeffect
CODE_CHECK_ARGS	=  	--enable=all
#
# Libraries and objects targets
#
OBJS  		     = 	$(OBJECT_DIR)/main.o  		\
			$(OBJECT_DIR)/interactive.o	\
			$(OBJECT_DIR)/parsecommandline.o

LIBS  		     = 	$(LIB_DIR)/liblister.a 		\
			$(LIB_DIR)/libtokenizer.a	\
			$(LIB_DIR)/libutilities.a

#*******************************************************************************
# Build targets:
# all		Creates object directory, builds executable test harness  and
#               runs checker
# lib		Build only the list library, no test harness
# splint-it	run the Syntax checker
# clean		Delete object and library files and intermediates
#*******************************************************************************

all:	$(OBJECT_DIR) $(LIB_DIR) $(LIBS) basic.exe test_harness splint-it

lib:	$(LIBS)

$(LIB_DIR):
	-$(MAKE_LIB_CMD)

$(OBJECT_DIR):
	-$(MAKE_DIR_CMD)

basic.exe:	$(OBJS) $(LIBS)
	$(LINK) $(OBJS) $(LFLAGS) -llister -lutilities -ltokenizer -o basic.exe

#*******************************************************************************
# Major library components
#*******************************************************************************
$(LIB_DIR)/liblister.a:	$(OBJECT_DIR)/lister.o 
	$(AR) rcs $(LIB_DIR)/liblister.a $(OBJECT_DIR)/lister.o
$(LIB_DIR)/libutilities.a:	$(OBJECT_DIR)/utilities.o 
	$(AR) rcs $(LIB_DIR)/libutilities.a $(OBJECT_DIR)/utilities.o
$(LIB_DIR)/libtokenizer.a:	$(OBJECT_DIR)/tokenizer.o 
	$(AR) rcs $(LIB_DIR)/libtokenizer.a $(OBJECT_DIR)/tokenizer.o

#*******************************************************************************
# Object builds
#*******************************************************************************
$(OBJECT_DIR)/main.o:		main.c
	$(CC) $(CFLAGS) $(DEBUG) main.c -o $(OBJECT_DIR)/main.o
$(OBJECT_DIR)/lister.o:		lister.c
	$(CC) $(CFLAGS) $(DEBUG) lister.c -o $(OBJECT_DIR)/lister.o
$(OBJECT_DIR)/utilities.o:	utilities.c
	$(CC) $(CFLAGS) $(DEBUG) utilities.c -o $(OBJECT_DIR)/utilities.o
$(OBJECT_DIR)/tokenizer.o:	tokenizer.c
	$(CC) $(CFLAGS) $(DEBUG) tokenizer.c -o $(OBJECT_DIR)/tokenizer.o
$(OBJECT_DIR)/interactive.o:	interactive.c
	$(CC) $(CFLAGS) $(DEBUG) interactive.c -o $(OBJECT_DIR)/interactive.o
$(OBJECT_DIR)/parsecommandline.o:	parsecommandline.c
	$(CC) $(CFLAGS) $(DEBUG) parsecommandline.c -o $(OBJECT_DIR)/parsecommandline.o

#
# This is the "checkmk" target: Test harness is in stack_check.ts file and 
# this is converted by "check" into a C file which is linked to give another
# executable. 
# 
# NOTE: This will not build if you have the Profiling enabled as the libstack.a 
# contains gcov 
#
test_harness: 
#	@echo "** test harness TODO **"
ifndef CHECK_FOR_CHK
	@echo "** checkmk command not found"
else
	$(CHK_TOOL) basic_check.ts > basic_check.c
	$(CC)  basic_check.c 	 \
	$(OBJECT_DIR)/interactive.o		 \
	-static -L$(LIB_DIR) 			 \
	-lcheck -llister -lutilities -ltokenizer \
	-o basic_check.exe
endif

#
# Code syntax checking target
#
splint-it:
ifndef CHECK_FOR_CPP
	@echo "** cppcheck command not found"
else
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) lister.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) utilities.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) main.c
	$(CODE_CHECK) $(CODE_CHECK_ARGS) *.c
endif

# remove all libs, objs and intermediates
clean:
	rm -f basic.exe
	rm -f basic_check.exe
	rm -f $(LIB_DIR)/liblister.a
	rm -f $(LIB_DIR)/libutilities.a
	rm -f $(LIB_DIR)/libtokenizer.a
	rm -f $(OBJECT_DIR)/main.o
	rm -f $(OBJECT_DIR)/lister.o
	rm -f $(OBJECT_DIR)/utilities.o
	rm -f $(OBJECT_DIR)/tokenizer.o
	rm -f $(OBJECT_DIR)/interactive.o
	rm -f $(OBJECT_DIR)/parsecommandline.o
	rm -f *.gcno
	rm -f *.gcda
	rm -f gmon.out
	rm -f core

#
# Fin
#

