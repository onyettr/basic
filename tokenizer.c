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
static Literal_t Literal;

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
 * @details   Builds the value of the number as a literal integer and not ascii. 
 * @note
 * @todo      Floating point and MAX_INT overflow
 */
Token_t TokenGetNumber(char **Bufferp, char *Tokenp) {
  char *Bufp;
  int value = 0;

  Bufp = *Bufferp;  

  if (Verbose) printf("TokenGetNUmber\n");
  
  do {
    value = 10 * value + (*Bufp -'0');
    *Tokenp++ = *Bufp++;
  } while (isdigit(*Bufp));


  Literal.Type  = LITERAL_INTEGER;
  Literal.value.IntegerValue = value;

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
 * @details
 * @todo
 */
Token_t TokenGetWord  (char **Bufferp, char *Tokenp) {
  char *Bufp;

  Bufp = *Bufferp;
  
  if (Verbose) printf("TokenGetWord %c\n", (int)*Bufp);
  
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
    Token_t TokenReturn;

    if (Verbose) printf("TokenGetSpecial\n");    
    Bufp = *Bufferp;

    *Tokenp++ = *Bufp;

    TokenReturn = (*Bufp == '.') ? TOKEN_SPECIAL : TOKEN_ERROR;

    Bufp++;
    *Tokenp = '\0';

    *Bufferp = Bufp;
    
    return TokenReturn;    
}

/**
 * @brief     Token types to string convert
 * @fn        char *TokenGetStringType(Token_t Token)
 * @param[in] Token
 * @return    char* string 
 * @note
 */
char *TokenGetStringType(Token_t Token) {

  switch (Token) {
     case TOKEN_WORD:   return ("<WORD>"); break;
     case TOKEN_DIGIT:  return ("<DIGIT>"); break;
     case TOKEN_LETTER: return ("<LETTER>"); break;
     case TOKEN_SPECIAL:return ("<SPECIAL>"); break;       
     case TOKEN_ERROR:  return ("<ERROR>"); break;
     default: return ("????"); break;
  }
}

/**
 * @brief     Tokenize a given file 
 * @fn        int32_t Tokenize (char *FileName)
 * @param[in] FileName - file containing lines to Tokenize
 * @return    int32_t ErrorCode
 * @note
 */
int32_t Tokenize (char *FileName) {
  FILE *fp;
  int32_t ErrorCode = SUCCESS;
  char *Bufferp;
  char *Tokenp;
  Token_t Token;  

  fp = fopen(FileName, "r");
  if ( fp == NULL ) {
    Error("[ERR] Failed to open %s", FileName);

    return ERROR_FILE_OPEN_FAILURE;    
  }
    
  Bufferp = SourceBuffer;
  Tokenp  = TokenBuffer;

  /*
   * read until EOF
   */
  while (UtilsReadSourceLine(fp, Bufferp) == true) {
     Bufferp = UtilsSkipSpaces(Bufferp);

     printf(">> %s", Bufferp);
     
     /* 
      * Parse the single line until the EOL
      */
     //
     //     while (*Bufferp != '\0' && Token != TOKEN_ERROR) {
     while (*Bufferp != '\0') {     
       if (isdigit(*Bufferp)) {
         Token = TokenGetNumber(&Bufferp, Tokenp);
       } else if (isalnum(*Bufferp)) {
         Token = TokenGetWord(&Bufferp, Tokenp);
       } else if (isspace(*Bufferp)) {
         Bufferp++;
       } else if (*Bufferp == '\n' || *Bufferp == '\r') {
         Bufferp++;
       } else {
         Token = TokenGetSpecial(&Bufferp, Tokenp);       
       }

       if (*Tokenp != '\0') {
         printf("\t>> %9s %s", TokenGetStringType(Token), Tokenp);
         if (Token == TOKEN_DIGIT ) {
           printf("   INTEGER = %d", Literal.value.IntegerValue);
         }
         printf("\n");
       }
       memset(TokenBuffer, '\0', sizeof(TokenBuffer));
     }
  }

  UNUSED(Token);
  
  return ErrorCode;
}
