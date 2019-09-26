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
  char *Bufferp;
  Token_t Token = TOKEN_OK;
  
  /*
   * read until Exit
   */
  while (Token != TOKEN_BYE) {
    int i = 0;
    int ch;

    Bufferp = SourceBuffer;

    memset(SourceBuffer,'\0', sizeof(SourceBuffer));    
    printf("basic> "); fflush(stdout);                    /* Sign on with a prompt                     */

    do {
      ch = getchar();
      SourceBuffer[i] = ch;
      i++;
    }  while (ch != '\n');
  
    Bufferp = UtilsSkipSpaces(Bufferp);

    while ((*Bufferp != '\0' && Token != TOKEN_BYE)) {
      if (isdigit(*Bufferp)) {                             /* Test for Numbers                         */
	Token = TokenGetNumber(&Bufferp, TokenBuffer);
      } else if (isalnum(*Bufferp)) {                      /* Test for Numbers and Letters             */
	Token = TokenGetWord(&Bufferp, TokenBuffer);
      } else if (isspace(*Bufferp)) {                      /* Test for SPACE, we just skip             */
	Bufferp++;
	Token = TOKEN_SPACE;
      } else if (*Bufferp == '"') {                        /* Test for STRINGS                         */
	Token = TokenGetString(&Bufferp, TokenBuffer);
      } else if (*Bufferp == '\n' || *Bufferp == '\r') {   /* TODO: convert to spaces for TokenBuffer? */
	Bufferp++;
      } else {                                             /* Test for Special characters              */
         Token = TokenGetSpecial(&Bufferp, TokenBuffer);
      }

      if (Token == TOKEN_WORD) {                           /* Test for direct command                  */
        Token = TokenDirectCommand(TokenBuffer);
        printf("Token last %s\n", TokenGetStringType(Token));
      }
      TokenPrint(TokenBuffer, Token);                      /* Show the Token buffer contentst           */
              
      memset(TokenBuffer, '\0', sizeof(TokenBuffer));      /* Clear Token buffer on each line parse     */
    }
  }
  TokenPrint(TokenBuffer, Token);  
  
  return ErrorCode;
}
