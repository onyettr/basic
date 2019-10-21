#*******************************************************************************
# File name        : makefile
# File description : basic interpreter
# Author           : ronyett
#*******************************************************************************

# Base location defaults
SRC_DIR		= 	.
SRC             =  	./src
OBJECT_DIR	= 	$(SRC_DIR)/object
LIB_DIR		= 	$(SRC_DIR)/libraries
INC_DIR		= 	$(SRC_DIR)/include
MAKE_DIR_CMD	= 	mkdir $(OBJECT_DIR)
MAKE_LIB_CMD	= 	mkdir $(LIB_DIR)

# Tools and options
CC  		= 	gcc
LINK  		= 	gcc
AR		= 	ar
CHK_TOOL	= 	checkmk
#CODE_CHECK       = 	splint
CLANG		= 	clang
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
CFLAGS		= 	-c -std=c99 -Wall -pedantic $(PFLAGS) -I $(INC_DIR)
OS_NAME		:=	$(shell uname -s)
ifeq ($(OS_NAME),Linux)
LFLAGS		= 	$(PFLAGS) -L. -L./$(LIB_DIR) -lm
else
LFLAGS		= 	$(PFLAGS) -static -L. -L./$(LIB_DIR) -lm
endif

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
			$(OBJECT_DIR)/error.o		\
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
#
# lister                  Pretty prints the code
# utilities               Small utilities and support functions
# tokenizer               Routines to do the parsing
#*******************************************************************************

$(LIB_DIR)/liblister.a:	$(OBJECT_DIR)/lister.o 
	$(AR) rcs $(LIB_DIR)/liblister.a $(OBJECT_DIR)/lister.o
$(LIB_DIR)/libutilities.a:	$(OBJECT_DIR)/utilities.o $(OBJECT_DIR)/binarytree.o
	$(AR) rcs $(LIB_DIR)/libutilities.a $(OBJECT_DIR)/utilities.o $(OBJECT_DIR)/binarytree.o
$(LIB_DIR)/libtokenizer.a:	$(OBJECT_DIR)/tokenizer.o 
	$(AR) rcs $(LIB_DIR)/libtokenizer.a $(OBJECT_DIR)/tokenizer.o

#*******************************************************************************
# Object builds
#*******************************************************************************

$(OBJECT_DIR)/main.o:		 $(SRC)/misc/main.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/misc/main.c -o $(OBJECT_DIR)/main.o
$(OBJECT_DIR)/error.o:		 $(SRC)/misc/error.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/misc/error.c -o $(OBJECT_DIR)/error.o
$(OBJECT_DIR)/lister.o:		 $(SRC)/misc/lister.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/misc/lister.c -o $(OBJECT_DIR)/lister.o
$(OBJECT_DIR)/utilities.o:	 $(SRC)/utilities/utilities.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/utilities/utilities.c -o $(OBJECT_DIR)/utilities.o
$(OBJECT_DIR)/tokenizer.o:	 $(SRC)/tokenizer/tokenizer.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/tokenizer/tokenizer.c -o $(OBJECT_DIR)/tokenizer.o
$(OBJECT_DIR)/interactive.o:	 $(SRC)/misc/interactive.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/misc/interactive.c -o $(OBJECT_DIR)/interactive.o
$(OBJECT_DIR)/symboltable.o:	 $(SRC)/utilities/symboltable.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/utilities/symboltable.c -o $(OBJECT_DIR)/symboltable.o
$(OBJECT_DIR)/binarytree.o:	 $(SRC)/utilities/binarytree.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/utilities/binarytree.c -o $(OBJECT_DIR)/binarytree.o
$(OBJECT_DIR)/parsecommandline.o:$(SRC)/misc/parsecommandline.c
	$(CC) $(CFLAGS) $(DEBUG) $(SRC)/misc/parsecommandline.c -o $(OBJECT_DIR)/parsecommandline.o

#*******************************************************************************
# This is the "checkmk" target: Test harness is in stack_check.ts file and 
# this is converted by "check" into a C file which is linked to give another
# executable. 
# 
# NOTE: This will not build if you have the Profiling enabled as the libstack.a 
# contains gcov 
#*******************************************************************************
test_harness: 
#	@echo "** test harness TODO **"
ifndef CHECK_FOR_CHK
	@echo "** checkmk command not found"
else
	$(CHK_TOOL) basic_check.ts > basic_check.c
	$(CC)  basic_check.c 	 			\
	-std=c99 -Wall -pedantic -I $(INC_DIR)		\
	$(OBJECT_DIR)/interactive.o			\
	$(OBJECT_DIR)/error.o		 		\
	$(LFLAGS)					\
	-lutilities -ltokenizer -llister 		\
	-lcheck						\
	-lpthread				 	\
	-o basic_check.exe
endif

#*******************************************************************************
# clang target
#*******************************************************************************
clang:
	$(CLANG) $(CFLAGS) $(DEBUG) src/misc/main.c       	-o $(OBJECT_DIR)/main_clang.o
	$(CLANG) $(CFLAGS) $(DEBUG) src/misc/lister.c      	-o $(OBJECT_DIR)/lister_clang.o
	$(CLANG) $(CFLAGS) $(DEBUG) src/misc/interactive.c 	-o $(OBJECT_DIR)/interactive_clang.o
	$(CLANG) $(CFLAGS) $(DEBUG) src/utilities/utilities.c   -o $(OBJECT_DIR)/utilities_clang.o
	$(CLANG) $(CFLAGS) $(DEBUG) src/tokenizer/tokenizer.c   -o $(OBJECT_DIR)/tokenizer_clang.o

#*******************************************************************************
# splint target
#*******************************************************************************
splint-it:
ifndef CHECK_FOR_CPP
	@echo "** cppcheck command not found"
else
	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/main.c
	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/error.c
	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/lister.c
	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/interactive.c
	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/parsecommandline.c
	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/tokenizer/tokenizer.c
	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/utilities/binarytree.c
	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/utilities/utilities.c
endif

#*******************************************************************************
# remove all libs, objs and intermediates
#*******************************************************************************
clean:
	rm -f basic.exe
	rm -f basic_check.exe
	rm -f basic_check.c
	rm -f $(LIB_DIR)/liblister.a
	rm -f $(LIB_DIR)/libutilities.a
	rm -f $(LIB_DIR)/libtokenizer.a
	rm -f $(OBJECT_DIR)/main.o
	rm -f $(OBJECT_DIR)/error.o
	rm -f $(OBJECT_DIR)/lister.o
	rm -f $(OBJECT_DIR)/utilities.o
	rm -f $(OBJECT_DIR)/tokenizer.o
	rm -f $(OBJECT_DIR)/binarytree.o
	rm -f $(OBJECT_DIR)/symboltable.o
	rm -f $(OBJECT_DIR)/interactive.o
	rm -f $(OBJECT_DIR)/parsecommandline.o
	rm -f *.gcno
	rm -f *.gcda
	rm -f gmon.out
	rm -f core

#
# Fin
#

