#*******************************************************************************
# File name        : makefile
# File description : basic interpreter
# Author           : ronyett
#*******************************************************************************

# Base location defaults
SRC_DIR				= 	.
SRC        	 	=  	./src
OBJECT_DIR		= 	$(SRC_DIR)/object
LIB_DIR				= 	$(SRC_DIR)/libraries
INC_DIR				= 	$(SRC_DIR)/include
MAKE_DIR_CMD	= 	mkdir $(OBJECT_DIR)
MAKE_LIB_CMD	= 	mkdir $(LIB_DIR)

OS_NAME				:=	$(shell uname -s)
BRANCH_NAME_STRING := $(shell git rev-parse --abbrev-ref HEAD)	

# Tools and options
CC  					= gcc
LINK 				 	= gcc
AR						= ar
CHK_TOOL			= checkmk
#CODE_CHECK       = 	splint
CLANG					= clang
CODE_CHECK		= cppcheck
CODE_SPELL  	= codespell
CODE_SPELL_ARGS = --skip="*.o,*.a,*.lib,*.elf,*.bin,*.exe,./.git,./tests,./html" 
CHECK_FOR_CHK		:= 	$(shell command -v $(CHK_TOOL)   2> /dev/null)
CHECK_FOR_CPP		:=	$(shell command -v $(CODE_CHECK) 2> /dev/null)
CHECK_FOR_SPELL := 	$(shell command -v $(CODE_SPELL) 2> /dev/null)

#*******************************************************************************
# Build options
#*******************************************************************************

# gcov and gprof build options
COVPFLAGS			= 	-fprofile-arcs -ftest-coverage
PROFLAGS			= 	-pg
#PFLAGS				= 	$(COVFLAGS)

# Main CC and Link build strings
DEBUG					= 	-g
CFLAGS				= 	-c -std=c99 -Wall -pedantic $(PFLAGS) -I $(INC_DIR)

CFLAGS				+= -DBRANCH_NAME=$(BRANCH_NAME_STRING)
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
OBJS  		   = 	$(OBJECT_DIR)/parsecommandline.o\
								$(OBJECT_DIR)/interactive.o			\
								$(OBJECT_DIR)/banner.o					\
								$(OBJECT_DIR)/error.o						\
								$(OBJECT_DIR)/main.o  					\

LIBS  		   = 	$(LIB_DIR)/liblister.a 					\
								$(LIB_DIR)/libtokenizer.a				\
								$(LIB_DIR)/libutilities.a

HDRS		     =  $(INC_DIR)/basic.h							\
								$(INC_DIR)/error.h							\
								$(INC_DIR)/lister.h							\
								$(INC_DIR)/tokenizer.h					\
								$(INC_DIR)/utilities.h					\
								$(INC_DIR)/symboltable.h

#*******************************************************************************
# Build targets:
# all		Creates object directory, builds executable test harness  and
#               runs checker
# lib		Build only the list library, no test harness
# splint-it	run the Syntax checker
# clean		Delete object and library files and intermediates
#*******************************************************************************

all:	$(OBJECT_DIR) $(LIB_DIR) $(LIBS) basic.exe test_harness spellcheck-it

lib:	$(LIBS)

$(LIB_DIR):
	-$(MAKE_LIB_CMD)

$(OBJECT_DIR):
	-$(MAKE_DIR_CMD)

basic.exe:	$(OBJS) $(LIBS)
	@echo 'Linking...'
	@$(LINK) $(OBJS) $(LFLAGS) -ltokenizer -lutilities -llister -o basic.exe

#*******************************************************************************
# Major library components
#
# lister                  Pretty prints the code
# utilities               Small utilities and support functions
# tokenizer               Routines to do the parsing
#*******************************************************************************

$(LIB_DIR)/liblister.a:	$(OBJECT_DIR)/lister.o 
	@$(AR) rcs $(LIB_DIR)/liblister.a $(OBJECT_DIR)/lister.o

$(LIB_DIR)/libutilities.a:	$(OBJECT_DIR)/utilities.o $(OBJECT_DIR)/binarytree.o
	@$(AR) rcs $(LIB_DIR)/libutilities.a $(OBJECT_DIR)/utilities.o $(OBJECT_DIR)/binarytree.o

$(LIB_DIR)/libtokenizer.a:	$(OBJECT_DIR)/tokenizer.o $(OBJECT_DIR)/dtss.o	
	@$(AR) rcs $(LIB_DIR)/libtokenizer.a $(OBJECT_DIR)/tokenizer.o $(OBJECT_DIR)/dtss.o	

#*******************************************************************************
# Object builds
#*******************************************************************************

$(OBJECT_DIR)/symboltable.o:	 $(SRC)/utilities/symboltable.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"
	
$(OBJECT_DIR)/binarytree.o:	 $(SRC)/utilities/binarytree.c  $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/tokenizer.o:	 $(SRC)/tokenizer/tokenizer.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/utilities.o:	 $(SRC)/utilities/utilities.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/parsecommandline.o:$(SRC)/misc/parsecommandline.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/interactive.o:	 $(SRC)/misc/interactive.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/lister.o:		 $(SRC)/misc/lister.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/error.o:		 $(SRC)/misc/error.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/banner.o:		 $(SRC)/misc/banner.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/dtss.o:		 $(SRC)/misc/dtss.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

$(OBJECT_DIR)/main.o:		 $(SRC)/misc/main.c $(HDRS)
	@echo 'Compiling file: $<' 
	@$(CC) $(CFLAGS) $(DEBUG) -o "$@" "$<"

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
	@$(CC) basic_check.c 	 						\
				-std=c99 -I $(INC_DIR)			\
				$(OBJECT_DIR)/interactive.o	\
				$(OBJECT_DIR)/error.o		 		\
				$(LFLAGS)										\
				-lutilities 								\
				-ltokenizer 								\
				-llister 										\
				-lcheck											\
				-lpthread				 						\
				-o basic_check.exe
endif

#*******************************************************************************
# clang target
#*******************************************************************************
clang:
	$(CLANG) $(CFLAGS) $(DEBUG) src/misc/main.c 	-o $(OBJECT_DIR)/main_clang.o
	$(CLANG) $(CFLAGS) $(DEBUG) src/misc/lister.c	-o $(OBJECT_DIR)/lister_clang.o
	$(CLANG) $(CFLAGS) $(DEBUG) src/misc/interactive.c -o $(OBJECT_DIR)/interactive_clang.o
	$(CLANG) $(CFLAGS) $(DEBUG) src/utilities/utilities.c -o $(OBJECT_DIR)/utilities_clang.o
	$(CLANG) $(CFLAGS) $(DEBUG) src/tokenizer/tokenizer.c -o $(OBJECT_DIR)/tokenizer_clang.o

#*******************************************************************************
# splint target
#*******************************************************************************
splint-it:
ifndef CHECK_FOR_CPP
	@echo "** cppcheck command not found"
else
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/main.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/error.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/lister.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/interactive.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/misc/parsecommandline.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/tokenizer/tokenizer.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/utilities/binarytree.c
#	$(CODE_CHECK) $(CODE_CHECK_ARGS) $(SRC)/utilities/utilities.c
	$(CODE_CHECK) --language=c --enable=all -v -I ./include src/*
endif

#*******************************************************************************
# spellcheck target
#*******************************************************************************
spellcheck-it:
ifndef CHECK_FOR_SPELL
	@echo "** codespell command not found"
else
	$(CODE_SPELL) $(CODE_SPELL_ARGS)
endif

#*******************************************************************************
# help
#*******************************************************************************

help:
	@echo "make all           - build libraries, executable "
	@echo "make libs          - build libraries             "
	@echo "make clean         - clean objects and executable"
	@echo "make spellcheck-it - run spelling check          "
	@echo "make splint-it     - run cppcheck                "
	@echo "make clang         - run clang, no exe generated "
	@echo "make test_harness  - build checkmk test harness  "
	@echo "make display       - environment details         "

#*******************************************************************************
# remove all libs, objs and intermediates
#*******************************************************************************
display:
	@echo "make variables"	
	@echo "OS_NAME            " $(OS_NAME)
	@echo "BRANCH_NAME_STRING " $(BRANCH_NAME_STRING)
	@echo "CC                 " $(CC)
	@echo "LINK               " $(LINK)

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
	rm -f $(OBJECT_DIR)/dtss.o
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

