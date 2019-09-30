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
#include "error.h"
#include "tokenizer.h"
#include "interactive.h"

#define PROGRAM_NAME            "basic"                        /*!> No name. no slogan          */
#define VERSION                 "(v1.0.0) DEVELOPMENT Version" /*!> Version                     */
#define AUTHOR_STRING           "()"                           /*!> Who did this?               */

/**
 * @brief control codes
 */
#define BELL                    '\007'                         /*!> Console Bell                */
#define FORM_FEED               '\014'                         /*!> Page break                  */

/**
 * @brief macros
 */
#define UNUSED( X )             (void)X
#define DIM( X )                (sizeof(X) / sizeof(X[0])

/**
 * @brief Constants
 */
#define MAX_DIGIT_COUNT         9                              /*!> Accuracy of original machine */


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
extern bool Verbose;                 /*! CLI option for more output   */
extern bool Secret;                  /*! CLI option for shhhh         */
extern bool ListMode;                /*! CLI option to enable listing */
extern bool ScanMode;    

//extern char SourceFileName[MAX_FILENAME_LENGTH];
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
