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

#define PROGRAM_NAME            "basic"                 /* No name. no slogan */
#define VERSION                 "(v1.0.0) DEVELOPMENT Version"
#define AUTHOR_STRING           "()"
#define BELL                    '\007'

#define MAX_FILENAME_LENGTH     32
#define MAX_SOURCE_LINE_LENGTH  256

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

extern FILE *SourceFile;

extern char SourceFileName[MAX_FILENAME_LENGTH];
extern char SourceLineBuffer[MAX_SOURCE_LINE_LENGTH];
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
extern void Error ( char *format, ... );

extern bool ReadSourceLine   ( FILE *sourceFile );
extern void OutputSourceLine ( char *Bufferp );
extern void SourceLister     ( char *Filename );
extern void WritePrologueHeader(char *Filename);

#endif  /* __BASIC_H__ */
