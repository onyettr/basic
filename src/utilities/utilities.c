/**
 *****************************************************************************
 * @brief   basic interpreter - list programme
 * @author  onyettr
 * @file    utilities.c
 * @ingroup Utilities
 ***************************************************************************** 
 */

/*
******************************************************************************
Includes
******************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "basic.h"

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
Private Macros
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
Prototypes of all functions contained in this file (in order of occurrence)
******************************************************************************
*/

/**
 * @fn          bool UtilsReadSourceLine ( FILE *sourceFile, char* ) {
 * @brief       Read a line of source from a file
 * @param[in]   *sourceFile - opened source file handle
 * @param[out   *SourceBuffer - place to store
 * @return      true if line read was ok, false if error or EOF
 * @details     Reads from opened file sourceFile and returns a single line in SourceBuffer
 * @todo  
 * @note        none
 */
bool UtilsReadSourceLine (FILE *sourceFile, char *SourceBuffer) {

  if (fgets((char *)SourceBuffer,MAX_SOURCE_LINE_LENGTH, sourceFile) != NULL ) {
      return true;
   }

   return false;
}

/**
 * @fn          char *UtilsSkipSpaces (char *SourceBuffer) 
 * @brief       Skip a given buffer of any spaces
 * @param[in]   *SourceBuffer 
 * @return      char * updated pointer into the source buffer
 * @note        none
 * @details     Skips spaces in a given string and returns pointer to first non space elemento
 */
char *UtilsSkipSpaces (char *SourceBuffer) {
  char *Startp = SourceBuffer;

   while (isspace(*Startp)) {   
      Startp++;
   }

   return Startp;
}

/**
 * @fn          bool isendofline(char *SourceBuffer)
 * @brief       Test for End of Line condition
 * @param[in]   *SourceBuffer 
 * @return      true if EOL, false otherwise
 * @note        none
 * @details     
 * @todo        
 */
bool isEndOfLine(char *SourceBuffer) {
   char *Startp = SourceBuffer;

   if (*Startp == '\0' || *Startp == '\n') {
     return true;
   }

   return false;
}


/**
 * @fn          void UtilsToUpper(char *SourceBuffer) {
 * @brief       Convert input string to upper case
 * @param[in]   *SourceBuffer 
 * @return      none
 * @note        none
 * @details     
 * @todo        
 */
void UtilsToUpper(char *SourceBuffer) {
  while (*SourceBuffer != '\0') {
    *SourceBuffer = toupper(*SourceBuffer);
    SourceBuffer++;
  }
}
