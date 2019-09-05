/**
 *****************************************************************************
 * @brief   basic interpreter
 * @author  onyettr
 * @file    basic.h
 ***************************************************************************** 
 */
#ifndef __BASIC_H__
#define __BASIC_H__

/*
******************************************************************************
Includes
******************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"
#include "lister.h"

#define PROGRAM_NAME            "basic"                 /* No name. no slogan */
#define VERSION                 "(v1.0.0) DEVELOPMENT Version"
#define AUTHOR_STRING           "()"
#define BELL                    '\007'
#define FORM_FEED               '\014'
#define MAX_FILENAME_LENGTH     32
#define MAX_SOURCE_LINE_LENGTH  256
#define MAX_LINES_PER_PAGE      20

/*
 * Error codes
 */
#define SUCCESS                       1
#define FAILURE                       0
#define ERROR_FILE_OPEN_FAILURE     (-1)

#define UNUSED( X )            (void) X

/*
******************************************************************************
Private Types
*****************************************************************************
*/

/*
******************************************************************************
Private variables (static)
******************************************************************************
*/

/*
******************************************************************************
Global variables
******************************************************************************
*/
extern bool Verbose;
extern bool Secret;  
extern bool ListMode;

extern char SourceFileName[MAX_FILENAME_LENGTH];
extern uint32_t LineNumber;

/*
******************************************************************************
Exported Global variables
******************************************************************************
*/

/*
******************************************************************************
Prototypes of all functions contained in this file (in order of occurance)
******************************************************************************
*/

#endif  /* __BASIC_H__ */
