/**
 *****************************************************************************
 * @brief   basic interpreter
 * @author  onyettr
 * @file    basic.h
 ***************************************************************************** 
 */
#ifndef __UTILITIES__H__
#define __UTILITIES__H__

/*
******************************************************************************
Includes
******************************************************************************
*/

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
void Error (char *format, ...);

bool UtilsReadSourceLine (FILE *sourceFile, char *SourceBuffer);

#endif  /* __UTILITIES__H__ */
