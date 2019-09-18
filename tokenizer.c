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
 * @brief     Process Number Token
 * @fn        Token_t TokenGetNumber(char *Bufferp, char *Tokenp) 
 * @param[in] **Bufferp - Buffer to tokenize
 * @param[out]*Tokenp  - add to tokenized buffer
 * @return    Token_t 
 * @details   Builds the value of the number as a literal integer and not ascii. 
 * @note
 * @todo      Floating point and MAX_INT overflow
 * @addtogroup Tokenizer
 */
Token_t TokenGetNumber(char **Bufferp, char *Tokenp) {
  char *Bufp;
  int value = 0;

  Bufp = *Bufferp;  

  if (Verbose) printf("TokenGetNumber\n");
  
  do {
    value = 10 * value + (*Bufp -'0');
    *Tokenp++ = *Bufp++;
  } while (isdigit(*Bufp));


  Literal.Type  = LITERAL_INTEGER;
  Literal.value.IntegerValue = value;

  *Tokenp = '\0';
  *Bufferp = Bufp;
    
  return TOKEN_NUMBER;
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
 * @addtogroup Tokenizer
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
 * @brief     Process String
 * @fn        Token_t TokenGetString (char *Bufferp, char *Tokenp) 
 * @param[in] Bufferp - Buffer to tokenize
 * @param[out]Tokenp  - add to tokenized buffer
 * @return    Token_t 
 * @notes     Tokenizer ceases when a SPACE or end of line if found
 * @details
 * @todo
 * @addtogroup Tokenizer
 */
Token_t TokenGetString (char **Bufferp, char *Tokenp) {
  char *Bufp;

  Bufp = *Bufferp;
  
  if (Verbose) printf("TokenGetString %c\n", (int)*Bufp);
  
  while (!isEndOfLine(Bufp)) {
    if (*Bufp == '\'') {
      *Tokenp++ = *Bufp++;
      if (*Bufp == '\'') break;
    }
    *Tokenp++ = *Bufp++;
  }

  *Tokenp = '\0';

  Literal.Type  = LITERAL_STRING;  
  strcpy(Literal.value.StringValue, Tokenp);

  *Bufferp = Bufp;
  
  return TOKEN_STRING;
}

/**
 * @brief      Process Sepcial Token
 * @fn         Token_t TokenGetSpecial(char *Bufferp, char *Tokenp) 
 * @param[in]  Bufferp - Buffer to tokenize
 * @param[out] Tokenp  - add to tokenized buffer
 * @return     Token_t 
 * @notes     
 */
Token_t TokenGetSpecial(char **Bufferp, char *Tokenp) {
    char *Bufp;
    Token_t TokenReturn;

    if (Verbose) printf("TokenGetSpecial\n");    
    Bufp = *Bufferp;
    *Tokenp++ = *Bufp;

    switch (*Bufp) {
      case '~': TokenReturn = TOKEN_TILDE;         break;
      case '!': TokenReturn = TOKEN_PLING;         break;
      case '@': TokenReturn = TOKEN_AT;            break;
      case '#': TokenReturn = TOKEN_HASH;          break;
      case '$': TokenReturn = TOKEN_DOLLAR;        break;                
      case '%': TokenReturn = TOKEN_PERCENT;       break;
      case '^': TokenReturn = TOKEN_HAT;           break;
      case '&': TokenReturn = TOKEN_AMPERSAND;     break;
      case '*': TokenReturn = TOKEN_MULTIPLY;      break;
      case '(': TokenReturn = TOKEN_LPAREN;        break;
      case ')': TokenReturn = TOKEN_RPAREN;        break;
      case '-': TokenReturn = TOKEN_MINUS;         break;
      case '+': TokenReturn = TOKEN_PLUS;          break;
      case '=': TokenReturn = TOKEN_EQUAL;         break;
      case '_': TokenReturn = TOKEN_DASH;          break;
      case '[': TokenReturn = TOKEN_L_BRACKET;     break;
      case ']': TokenReturn = TOKEN_R_BRACKET;     break;
      case '{': TokenReturn = TOKEN_L_CURLY;       break;
      case '}': TokenReturn = TOKEN_R_CURLY;       break;
      case '/': TokenReturn = TOKEN_DIVIDE;        break;
      case '|': TokenReturn = TOKEN_VERTICAL_BAR;  break;        
      case '<': TokenReturn = TOKEN_LT;            break;
      case '>': TokenReturn = TOKEN_GT;            break;
      case ',': TokenReturn = TOKEN_COMMA;         break;
      case '\\':TokenReturn = TOKEN_BACK_SLASH;    break;
      case '"': TokenReturn = TOKEN_QUOTE;         break;
      case '\'':TokenReturn = TOKEN_SINGLE_QUOTE;  break;
      case '`': TokenReturn = TOKEN_OPEN_QUOTE;    break;        
      case '.': TokenReturn = TOKEN_PERIOD;        break;
      case ':': TokenReturn = TOKEN_COLON;         break;
      case ';': TokenReturn = TOKEN_SEMI_COLON;    break;                
      case '?': TokenReturn = TOKEN_QUESTION_MARK; break;
      default:  TokenReturn = TOKEN_ERROR;
    }
    //    TokenReturn = (*Bufp == '.') ? TOKEN_SPECIAL : TOKEN_ERROR;

    Bufp++;
    *Tokenp = '\0';

    *Bufferp = Bufp;
    
    return TokenReturn;    
}
/**
 * @brief      Process direct commands
 * @fn         Token_t TokenGetDirect(char **Bufferp, char *Tokenp) 
 * @param[in]  Bufferp - Buffer to tokenize
 * @param[out] Tokenp  - add to tokenized buffer
 * @return     Token_t 
 */
Token_t TokenGetDirect(char **Bufferp, char *Tokenp) {
    char *Bufp;
    Token_t TokenReturn;

    if (Verbose) printf("TokenGetDirect\n");    
    Bufp = *Bufferp;
    *Tokenp++ = *Bufp;

    UNUSED(TokenReturn);
    
    return TOKEN_ERROR;
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
     case TOKEN_WORD:          return ("<WORD>");       break;
     case TOKEN_STRING:        return ("<STRING>");     break;       
     case TOKEN_DIGIT:         return ("<DIGIT>");      break;
     case TOKEN_NUMBER:        return ("<NUMBER>");     break;       
     case TOKEN_LETTER:        return ("<LETTER>");     break;
     case TOKEN_SPECIAL:       return ("<SPECIAL>");    break;
     case TOKEN_NO_TOKEN:      return ("<NO TOKEN>");   break;
     case TOKEN_ERROR:         return ("<ERROR>");      break;
     case TOKEN_TILDE:         return ("<TILDE>");      break;
     case TOKEN_PLING:         return ("<PLING>");      break;
     case TOKEN_AT:            return ("<AT>");         break;
     case TOKEN_HASH:          return ("<HASH>");       break;
     case TOKEN_DOLLAR:        return ("<DOLLAR>");     break;       
     case TOKEN_PERCENT:       return ("<PERCENT>");    break;
     case TOKEN_HAT:           return ("<HAT>");        break;
     case TOKEN_AMPERSAND:     return ("<AMPERSAND>");  break;       
     case TOKEN_MULTIPLY:      return ("<MULTIPLY>");   break;
     case TOKEN_LPAREN:        return ("<LPAREN>");     break;
     case TOKEN_RPAREN:        return ("<RPAREN>");     break;
     case TOKEN_MINUS:         return ("<MINUS>");      break;
     case TOKEN_PLUS:          return ("<PLUS>");       break;
     case TOKEN_EQUAL:         return ("<EQUAL>");      break;
     case TOKEN_DIVIDE:        return ("<DIVIDE>");     break;       
     case TOKEN_DASH:          return ("<UNDERSCORE>"); break;
     case TOKEN_L_BRACKET:     return ("<LBRACKET>");   break;
     case TOKEN_R_BRACKET:     return ("<RBRACKET>");   break;
     case TOKEN_L_CURLY:       return ("<LCURLY>");     break;
     case TOKEN_R_CURLY:       return ("<RCURLY>");     break;
     case TOKEN_VERTICAL_BAR:  return ("<VERT BAR>");   break;
     case TOKEN_LT:            return ("<LT>");         break;
     case TOKEN_GT:            return ("<GT>");         break;
     case TOKEN_COMMA:         return ("<COMMA>");      break;
     case TOKEN_SEMI_COLON:    return ("<SEMI COLON>"); break;
     case TOKEN_COLON:         return ("COLON>");       break;       
     case TOKEN_QUOTE:         return ("<QUOTE>");      break; 
     case TOKEN_SINGLE_QUOTE:  return ("<SINGLE QUOTE>");break;
     case TOKEN_OPEN_QUOTE:    return ("<OPEN QUOTE>"); break;             
     case TOKEN_PERIOD:        return ("<PERIOD>");     break;
     case TOKEN_SPACE:         return ("<SPACE>");      break;
     case TOKEN_BACK_SLASH:    return ("<BACKSLASH>");  break;     
     case TOKEN_QUESTION_MARK: return ("<QUESTION>");   break;
     default:                  return ("????");         break;
  }
}

/**
 * @brief     Print the token string
 * @fn        Token_t TokenGetWord  (char *Bufferp, char *Tokenp) 
 * @param[in] *TokenString - actual token buffer
 * @param[in] Token        - The token found
 * @return    void
 * @details   Some tokens (SPACE) are ignored
 * @todo
 */
void TokenPrint (char *TokenString, Token_t Token) {

    if (*TokenString != '\0') {
      char *Return;

      printf("\t>> %16s   %s", TokenGetStringType(Token), TokenString);
      if (Token == TOKEN_NUMBER) {
  	printf("   (INTEGER) = %d", Literal.value.IntegerValue);
      }
      if (Token == TOKEN_STRING) {
  	printf("   (STRING ) = %s", Literal.value.StringValue);        
      }        
      Return = strchr(TokenString,'\n');
      if (Return == NULL) {
        printf("\n");
      }
    } else {
      if (Token == TOKEN_NO_TOKEN ) {
        printf("   NO TOKEN\n");
      }
      if (Token == TOKEN_EOF) {
        printf("   EOF\n");
      }
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

  if (*FileName == '\0') {
    Error("No filename provided");

    return ERROR_FILE_NO_FILENAME;
  }
  
  fp = fopen(FileName, "r");
  if ( fp == NULL ) {
    Error("Failed to open %s", FileName);

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
       } else if (*Bufferp == '"') {
         Token = TokenGetString(&Bufferp, Tokenp);
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
