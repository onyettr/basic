/**
 *****************************************************************************
 * @brief   basic interpreter - list programme
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
static char Buffer[MAX_SOURCE_LINE_LENGTH+10];

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
 * @fn          void SourceLister   ( char *Filename )
 * @brief       Opens a file, reads source and outputs
 * @param[out]  char*  InputFileName
 * @return      none
 * @note        
 */
void SourceLister   ( char *Filename ) {
  
  /*
   * Open the filename containing the source lines 
   */
  SourceFile = fopen(Filename, "r");
  if ( SourceFile == NULL ) {
    Error("Failed to open %s", Filename);
    return;
  }

  /*
   * Output prologue header
   */
  WritePrologueHeader(Filename);
  
  /*
   * Read each line until EOF
   */
  while (ReadSourceLine(SourceFile));
}

/**
 * @fn          bool ReadSourceLine ( FILE *sourceFile ) {
 * @brief       Read a line of source from a file
 * @param[in]   FILE * - opened source file
 * @return      true if line read was ok, false if error or EOF
 * @note        none
 */
bool ReadSourceLine ( FILE *sourceFile ) {


   if ( fgets(SourceLineBuffer,MAX_SOURCE_LINE_LENGTH, sourceFile) != NULL ) {
     LineNumber++;
  
     sprintf(Buffer, "%4d: %s", LineNumber, SourceLineBuffer);

     OutputSourceLine( Buffer );

     return true;
   } else {
     return false;
   }
}

/**
 * @fn          void OutputSourceLine ( char *Bufferp ) {
 * @brief       Output a source line
 * @param[in]   char * Bufferp - buffer containing a line of source
 * @return      none
 * @note        none
 */
void OutputSourceLine ( char *Bufferp ) {
  printf("%s", Bufferp);
}

void WritePrologueHeader(char *Filename) {
   time_t CurrentTime;
   struct tm *info;

   time(&CurrentTime);
   info = localtime(&CurrentTime);
   sprintf(Buffer, "Filename: %s, %s\n", Filename, asctime(info));

   printf("%s", Buffer);
}
