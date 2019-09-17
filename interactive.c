/**
 *****************************************************************************
 * @brief   basic interpreter - interactive
 * @author  onyettr
 * @file    interactive.c
 ***************************************************************************** 
 */

/*
******************************************************************************
Includes
******************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
static char TokenBuffer [MAX_SOURCE_LINE_LENGTH+10];
static char SourceBuffer[MAX_SOURCE_LINE_LENGTH+10];

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

Token_t DirectCommand(char *TokenString) {
  // Token_t Token;

  return TOKEN_ERROR;
}

/**
 * @brief     Tokenize a given input stream
 * @fn        int32_t CommandLine (void)
 * @param[in] 
 * @return    int32_t ErrorCode
 * @note
 */
int32_t CommandLineMode (void) {
  int32_t ErrorCode = SUCCESS;
  int ch;
  char *Bufferp;
  Token_t Token;  
  bool Parsing = true;
  
  /*
   * read until Exit
   */
  while (Parsing) {
    char *Tokenp;
    int i = 0;
      
    Bufferp = SourceBuffer;
    Tokenp  = TokenBuffer;

    memset(SourceBuffer,'\0', sizeof(SourceBuffer));    
    printf("basic> "); fflush(stdout);    

    do {
      ch = getchar();
      SourceBuffer[i] = ch;
      i++;
    }  while (ch != '\n');
  
    Bufferp = UtilsSkipSpaces(Bufferp);

    /* 
     * Parse the single line until the EOL
     */
    while (*Bufferp != '\0' && Token != TOKEN_ERROR) {     
      if (isdigit(*Bufferp)) {
        Token = TokenGetNumber(&Bufferp, Tokenp);
      } else if (isalnum(*Bufferp)) {
        Token = TokenGetWord(&Bufferp, Tokenp);
      } else if (isspace(*Bufferp)) {
        Bufferp++;
        Token = TOKEN_SPACE;
      } else if (*Bufferp == '\n' || *Bufferp == '\r') {
        Bufferp++;
        *Tokenp = ' ';
      } else {
        Token = TokenGetSpecial(&Bufferp, Tokenp);       
      }

      TokenPrint(TokenBuffer, Token);
       
      memset(TokenBuffer, '\0', sizeof(TokenBuffer));
    }
  }  

  Token = TOKEN_EOF;
  TokenPrint(TokenBuffer, Token);  
  
  return ErrorCode;
}
