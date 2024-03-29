/**  
 *****************************************************************************
 * @brief   basic interpreter
 * @author  onyettr
 * @file    utilities,h
 * @ingroup Utilities
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
/**
 * @brief constants
 */
#define MAX_FILENAME_LENGTH          32
#define MAX_SOURCE_LINE_LENGTH      256
#define MAX_LINES_PER_PAGE           20

/*
******************************************************************************
Prototypes of all functions contained in this file (in order of occurrence)
******************************************************************************
*/
void Error (char *format, ...);
void ParseCommandLine (char *argv[], int argc, char *InputFileName);
  
bool UtilsReadSourceLine (FILE *sourceFile, char *SourceBuffer);
char *UtilsSkipSpaces (char *SourceBuffer);
bool isendofline(char *SourceBuffer);
void UtilsToUpper(char *SourceBuffer);

#endif  /* __UTILITIES__H__ */
