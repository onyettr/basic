/**
 *****************************************************************************
 * @brief   basic interpreter - interactive
 * @author  onyettr
 * @file    interactive.c
 * @ingroup Misc
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

SymbolTableNode_t *symTable = NULL;

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

Token_t DirectCommand(char *TokenString) {
  // Token_t Token;

  return TOKEN_ERROR;
}

int32_t TT_SymbolTableShow (void) {
  SymbolTableShow(symTable);

  return SUCCESS;
}


/**
 * @brief     Tokenize a given input stream
 * @fn        int32_t CommandLineMode (void)
 * @return    int32_t ErrorCode
 * @note
 */
int32_t CommandLineMode (void) {
  int32_t ErrorCode = SUCCESS;
  char *Bufferp;
  Token_t Token = TOKEN_OK;
  SymbolTableNode_t *pNewNode;

  /*
   * read until Exit
   */
  while (Token != TOKEN_BYE) {
    int i = 0;
    int ch;

    Bufferp = SourceBuffer;

    memset(SourceBuffer,'\0', sizeof(SourceBuffer));    
    printf("basic> "); fflush(stdout);                    /* Sign on with a prompt                     */

    do {                                                  /* GRab characters until CR  LF              */
      ch = getchar();
      SourceBuffer[i] = ch;
      i++;
    }  while (ch != '\n');
  
    Bufferp = UtilsSkipSpaces(Bufferp);                    /* Eat any leading spaces                   */

    while ((*Bufferp != '\0' && Token != TOKEN_BYE)) {
      if ((Token == TOKEN_MINUS  && isdigit(*Bufferp+1)) || /* Test for Numbers or -ve                  */
	  (Token == TOKEN_PERIOD && isdigit(*Bufferp+1)) ||
	  isdigit(*Bufferp) || Token == TOKEN_PERIOD) {
	Token = TokenGetNumber(&Bufferp, TokenBuffer, Token);
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

      if (Token == TOKEN_WORD) {                           
        if (IsTokenDirectCommand(TokenBuffer)) {           /* Test for Direct Command                  */
           Token = TokenDirectCommand(TokenBuffer);        /* Which direct command?                    */
           TokenExecuteDirectCommand(Token, TokenBuffer);  /* Execute direct command                   */
        } else if (IsTokenDirectKeyword(TokenBuffer)) {    /* Test for a Keyword                       */
          printf("TODO Keyword...\n");
        } else {                                           /* This is an identifier                    */
          pNewNode = SymbolTableSearch  (TokenBuffer, symTable);
          if (pNewNode == NULL) {
            pNewNode = SymbolTableAddName(TokenBuffer, &symTable);
          }

          Token = TOKEN_IDENTIFIER;
        }
      }
      TokenPrint(TokenBuffer, Token);                      /* Show the Token buffer contentst          */
              
      memset(TokenBuffer, '\0', sizeof(TokenBuffer));      /* Clear Token buffer on each line parse    */
    }
  }
  TokenPrint(TokenBuffer, Token);

  if (symTable != NULL) {
    SymbolTableClean(symTable);
  }
  
  return ErrorCode;
}
