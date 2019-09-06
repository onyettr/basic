/**
 *****************************************************************************
 * @brief   basic interpreter - tokenizer
 * @author  onyettr
 * @file    tokenizer.c
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
//static char SourceLineBuffer[MAX_SOURCE_LINE_LENGTH];

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

/**
 * @brief     Process Special token
 * @fn        Token_t TokenGetNumber(char *Bufferp, char *Tokenp) 
 * @param[in] Bufferp - Buffer to tokenize
 * @param[out]Tokenp  - add to tokenized buffer
 * @return    Token_t 
 * @notes     
 */
Token_t TokenGetNumber(char **Bufferp, char *Tokenp) {
  char *Bufp;
  
  printf("TokenGetNUmber\n");
  Bufp = *Bufferp;
  
  while ( ((isalnum(*Bufp)) || (*Bufp != '\0')) && (!isspace(*Bufp)) ) {
      *Tokenp++ = *Bufp++;
  }
  *Tokenp = '\0';
  *Bufferp = Bufp;
    
  return TOKEN_DIGIT;
}

/**
 * @brief     Process word (alpha) token
 * @fn        Token_t TokenGetWord  (char *Bufferp, char *Tokenp) 
 * @param[in] Bufferp - Buffer to tokenize
 * @param[out]Tokenp  - add to tokenized buffer
 * @return    Token_t 
 * @notes     Tokenizer ceases when a SPACE or end of line if found
 */
Token_t TokenGetWord  (char **Bufferp, char *Tokenp) {
  char *Bufp;

  Bufp = *Bufferp;
  printf("TokenGetWord %c\n", *Bufp);
  
  while ( ((isalnum(*Bufp)) || (*Bufp != '\0')) && (!isspace(*Bufp)) ) {
      *Tokenp++ = *Bufp++;
#if 0
      //      printf("%c addr %p", *Bufp, Bufp);
      if (isalnum(*Bufp)) {
        printf("alpha ");
      } else if (isspace(*Bufp)) {
        printf("space ");
      }
#endif      
  }

  *Tokenp = '\0';
  *Bufferp = Bufp;
  
  return TOKEN_WORD;
}

/**
 * @brief     Process Special token
 * @fn        Token_t TokenGetSpecial(char *Bufferp, char *Tokenp) 
 * @param[in] Bufferp - Buffer to tokenize
 * @param[out]Tokenp  - add to tokenized buffer
 * @return    Token_t 
 * @notes     
 */
Token_t TokenGetSpecial(char **Bufferp, char *Tokenp) {
    char *Bufp;

    Bufp = *Bufferp;
    printf("TokenGetSpecial\n");

    while ( (!(isalnum(*Bufp)) && !(*Bufp != '\0')) && (!isspace(*Bufp)) ) {
      *Tokenp++ = *Bufp++;
    }

    *Tokenp = '\0';
    *Bufferp = Bufp;
    
    return TOKEN_SPECIAL;    
}

/**
 * @brief     Token types to string
 * @fn        char *TokenGetStringType(Token_t Token)
 * @param[in] Token
 * @return    char*
 * @notes     
 */
char *TokenGetStringType(Token_t Token) {

  switch (Token) {
     case TOKEN_WORD:   return("WORD"); break;
     case TOKEN_DIGIT:  return("DIGIT"); break;
     case TOKEN_LETTER: return ("LETTER"); break;
     case TOKEN_SPECIAL:return ("SPECIAL"); break;       
     case TOKEN_ERROR:  return ("ERROR"); break;
     default: return ("????"); break;
  }
}

/**
 * @brief     Tokenize a given file 
 * @fn        int32_t Tokenize (char *FileName)
 * @param[in] FileName - file containing lines to Tokenize
 * @return    int32_t ErrorCode
 * @notes     
 */
int32_t Tokenize (char *FileName) {
  FILE *fp;
  int32_t ErrorCode = SUCCESS;
  char *Bufferp;
  char *Tokenp;
  Token_t Token;  

  fp = fopen(FileName, "r");
    
  Bufferp = SourceBuffer;
  Tokenp  = TokenBuffer;
  
  while (UtilsReadSourceLine(fp, Bufferp) == true) {
     Bufferp = UtilsSkipSpaces(Bufferp);

     while (*Bufferp != '\0') {
     if (isdigit(*Bufferp)) {
       Token = TokenGetNumber(&Bufferp, Tokenp);
     } else if (isalnum(*Bufferp)) {
       Token = TokenGetWord(&Bufferp, Tokenp);
     } else if (isspace(*Bufferp)) {
       Bufferp++;
     } else {
       Token = TokenGetSpecial(&Bufferp, Tokenp);       
     }

     printf(">> %5s %s\n", TokenGetStringType(Token), Tokenp);
    }
  }

  UNUSED(TokenBuffer);
  UNUSED(Token);
  
  return ErrorCode;
}
