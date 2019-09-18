/**
 *****************************************************************************
 * @brief   basic interpreter - list programme to stdout
 * @author  onyettr
 * @file    lister.c
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
static char LineBuffer[MAX_SOURCE_LINE_LENGTH+10];
static char SourceLineBuffer[MAX_SOURCE_LINE_LENGTH];

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
uint32_t LineNumber = 0;

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
 * @fn          int32_t ListerSource( char *Filename )
 * @brief       Opens a file, reads source and outputs to stdout
 * @param[in]   *Filename - string containing the name of the source file
 * @return      int32_t ErrorCode
 * @note        closes the file before returning
 */
int32_t  ListerSource (char *Filename) {
  uint32_t PageNumber = 1;
  int32_t ErrorCode = SUCCESS;
  FILE *SourceFile;  

  /*
   * Open the filename containing the source lines 
   */
  SourceFile = fopen(Filename, "r");
  if (SourceFile == NULL) {
    Error("[ERR] Failed to open %s", Filename);

    return ERROR_FILE_OPEN_FAILURE;
  }

  /*
   * Output prologue header
   */
  ListerWritePrologueHeader(Filename);
  
  /*
   * Read each line until EOF
   */
  while (UtilsReadSourceLine(SourceFile,SourceLineBuffer) == true) {
     LineNumber++;

     /*
      * Page break after  a number of lines
      */
     if (LineNumber % MAX_LINES_PER_PAGE == 0) {
       printf ("%c\t\t\t\t\t  - %d - \n\n", FORM_FEED, (int)PageNumber);
       PageNumber++;
     }
     
     sprintf(LineBuffer, "%4d: %s", (int)LineNumber, SourceLineBuffer);

     ListerOutputSourceLine(LineBuffer);
  }

  fclose(SourceFile);

  return ErrorCode;
}

/**
 * @fn          void ListerOutputSourceLine ( char *Bufferp ) {
 * @brief       Output a source line to stdout
 * @param[in]   *Bufferp - buffer containing a line of source
 * @return      none
 * @note        none
 */
void ListerOutputSourceLine (char *Bufferp) {
  printf("%s", Bufferp);
}

/**
 * @fn          void ListerWritePrologueHeader(char *Filename) 
 * @brief       Add header to the source listing
 * @param[in]   *Filename - name of the file being processed
 * @return      none
 * @note        Adds the time/date to the header
 */
void ListerWritePrologueHeader(char *Filename) {
   time_t CurrentTime;
   struct tm *info;

   time(&CurrentTime);
   info = localtime(&CurrentTime);

   sprintf(LineBuffer, "Filename: %s\n", Filename);
   ListerOutputSourceLine(LineBuffer);   
   strftime(LineBuffer,80,"%x - %I:%M%p", info);
   ListerOutputSourceLine(LineBuffer);
   printf("\n");
}
