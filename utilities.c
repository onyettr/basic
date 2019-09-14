/**
 *****************************************************************************
 * @brief   basic interpreter - list programme
 * @author  onyettr
 * @file    utilities.c
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
Prototypes of all functions contained in this file (in order of occurance)
******************************************************************************
*/

/**
 * @fn          void Error ( char *format, ... ) {
 * @brief       standard error output formatter
 * @param[in]   *format, ... number of formatted argumantes
 * @return      none
 * @note        outputs error string
 */
void Error ( char *format, ... ) {
    va_list ap;
    char Buffer[256];

    va_start ( ap, format );
    (void)vsprintf ( Buffer, format, ap);
    va_end ( ap );

    printf ("Error-%s-%s\n", PROGRAM_NAME, Buffer );

    exit (0);
}
  
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

   if (fgets(SourceBuffer,MAX_SOURCE_LINE_LENGTH, sourceFile) != NULL ) {
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
bool isendofline(char *SourceBuffer) {
   char *Startp = SourceBuffer;

   if (*Startp == '\0' || *Startp == '\n') {
     return true;
   }

   return false;
}
