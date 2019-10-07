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
#if defined(DEBUG)
#define DEBUG(X) print(X)
#endif
/*
******************************************************************************
Global variables
******************************************************************************
*/
static char TokenBuffer [MAX_SOURCE_LINE_LENGTH+10];
static char SourceBuffer[MAX_SOURCE_LINE_LENGTH+10];
static Literal_t Literal;

/**
 *  @brief  All Direct (not keywords) commands
 *  @struct TokenCommandList_t TokenCommand
 */
static TokenCommandList_t TokenCommand[] = {

    { "HELLO"   , TOKEN_HELLO   , NULL },
    { "NEW"     , TOKEN_NEW     , NULL },
    { "OLD"     , TOKEN_OLD     , NULL },
    { "SAVE"    , TOKEN_SAVE    , NULL },
    { "REPLACE" , TOKEN_REPLACE , NULL },
    { "RENAME"  , TOKEN_RENAME  , NULL },
    { "CAT"     , TOKEN_CAT     , NULL },
    { "LIST"    , TOKEN_LIST    , NULL },
    { "RUN"     , TOKEN_RUN     , NULL },
    { "STOP"    , TOKEN_CMD_STOP, NULL },
    { "UNSAVE"  , TOKEN_UNSAVE  , NULL },
    { "SYSTEM"  , TOKEN_SYSTEM  , NULL },
    { "BYE"     , TOKEN_BYE     , NULL },
    { "GOODBYE" , TOKEN_GOODBYE , NULL },
    { NULL      , TOKEN_WORD    , NULL }    
};

/**
 *  @brief  All language reserved Keywords
 *  @struct TokenCommandList_t TokenKeyword
 */
static TokenCommandList_t TokenKeyword[] = {
    { "LET"     , TOKEN_LET     , NULL },
    { "PRINT"   , TOKEN_PRINT   , NULL },
    { "END"     , TOKEN_END     , NULL },
    { "READ"    , TOKEN_READ    , NULL },
    { "DATA"    , TOKEN_DATA    , NULL },
    { "GOTO"    , TOKEN_GOTO    , NULL },
    { "IF"      , TOKEN_IF      , NULL },
    { "THEN"    , TOKEN_THEN    , NULL },    
    { "FOR"     , TOKEN_FOR     , NULL },
    { "TO"      , TOKEN_TO      , NULL },    
    { "NEXT"    , TOKEN_NEXT    , NULL },
    { "GOSUB"   , TOKEN_GOSUB   , NULL },
    { "RETURN"  , TOKEN_RETURN  , NULL },
    { "DEF"     , TOKEN_DEF     , NULL },
    { "DIM"     , TOKEN_DIM     , NULL },    
    { "REM"     , TOKEN_REM     , NULL },
    { "MAT"     , TOKEN_MAT     , NULL },      /* 2nd edition */
    { "SGN"     , TOKEN_SGN     , NULL },      /* 3rd edition */
    { "RESTORE" , TOKEN_RESTORE , NULL },
    { "INPUT"   , TOKEN_INPUT   , NULL },
    { "RANDOMIZE",TOKEN_RANDOMIZE,NULL },      /* 4th edition */
    { "ON"      , TOKEN_ON      , NULL },
    { "CHANGE"  , TOKEN_CHANGE  , NULL },
    { "RESET"   , TOKEN_RESET   , NULL },      /* 5th edition */
    { "LOC"     , TOKEN_LOC     , NULL },
    { "LOF"     , TOKEN_LOF     , NULL },
    { "CHAIN"   , TOKEN_CHAIN   , NULL },
    { "COMMON"  , TOKEN_COMMON  , NULL },
    
    //    { "STOP"    , TOKEN_STOP    , NULL },
    { NULL      , TOKEN_WORD    , NULL }    
};

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
 * @brief     Match strings
 * @fn        static int StringMatch(char *str1, char *str2)
 * @param[in] *str1 
 * @param[in] *str2
 * @return    true if str1 matches str2, false otherwise
 * @details   
 * @note
 * @todo     
 */
static bool StringMatch(char *str1,char *str2) {
  int i;

  for (i=0; i < (int)strlen(str2); i++) {
    if (str1[i] != str2[i] )
      return false;           /* No match */
  }

  return(!(strncmp(str1,str2, strlen(str1))));
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
     case TOKEN_COMMA:         return ("<COMMA>");      break;
     case TOKEN_SEMI_COLON:    return ("<SEMI COLON>"); break;
     case TOKEN_COLON:         return ("<COLON>");      break;       
     case TOKEN_QUOTE:         return ("<QUOTE>");      break; 
     case TOKEN_SINGLE_QUOTE:  return ("<SNG  QUOTE>"); break;
     case TOKEN_OPEN_QUOTE:    return ("<OPEN QUOTE>"); break;             
     case TOKEN_PERIOD:        return ("<PERIOD>");     break;
     case TOKEN_SPACE:         return ("<SPACE>");      break;
     case TOKEN_BACK_SLASH:    return ("<BACKSLASH>");  break;     
     case TOKEN_QUESTION_MARK: return ("<QUESTION>");   break;
     case TOKEN_LT:            return ("<LT>");         break;
     case TOKEN_GT:            return ("<GT>");         break;
     case TOKEN_GE:            return ("<GE>");         break; 
     case TOKEN_LE:            return ("<LE>");         break;
     case TOKEN_NE:            return ("<NE>");         break;       
     case TOKEN_LET:           return ("<RW LET>");     break;      /* Reserved Language Words */
     case TOKEN_PRINT:         return ("<RW PRINT>");   break;
     case TOKEN_END:           return ("<RW END>");     break;
     case TOKEN_READ:          return ("<RW READ>");    break;
     case TOKEN_DATA:          return ("<RW DATA>");    break;
     case TOKEN_GOTO:          return ("<RW GOTO>");    break;
     case TOKEN_FOR:           return ("<RW FOR>");     break;
     case TOKEN_IF:            return ("<RW IF>");      break;
     case TOKEN_THEN:          return ("<RW THEN>");    break;
     case TOKEN_TO:            return ("<RW TO>");      break;       
     case TOKEN_NEXT:          return ("<RW NEXT>");    break;
     case TOKEN_GOSUB:         return ("<RW GOSUB>");   break;
     case TOKEN_RETURN:        return ("<RW RETURN>");  break;
     case TOKEN_DEF:           return ("<RW DEF>");     break;
     case TOKEN_DIM:           return ("<RW DIM>");     break;
     case TOKEN_REM:           return ("<RW REM>");     break;
     case TOKEN_MAT:           return ("<RW MAT>");     break;
     case TOKEN_SGN:           return ("<RW SGN>");     break;
     case TOKEN_RESTORE:       return ("<RW RESTORE>"); break;
     case TOKEN_INPUT:         return ("<RW INPUT>");   break;
     case TOKEN_RANDOMIZE:     return ("<RW RANDOMIZE>"); break;
     case TOKEN_ON:            return ("<RW ON>");      break;
     case TOKEN_CHANGE:        return ("<RW CHANGE>");  break;                     
     case TOKEN_CMD_STOP:       
     case TOKEN_STOP:          return ("<CMD STOP>");   break;
     case TOKEN_HELLO:         return ("<CMD HELLO>");  break;
     case TOKEN_NEW:           return ("<CMD NEW  >");  break;
     case TOKEN_OLD:           return ("<CMD OLD  >");  break;
     case TOKEN_SAVE:          return ("<CMD SAVE>");   break;
     case TOKEN_REPLACE:       return ("<CMD REPLACE>");break;
     case TOKEN_RENAME:        return ("<CMD RENAME>"); break;
     case TOKEN_CAT:           return ("<CMD CAT>");    break;
     case TOKEN_LIST:          return ("<CMD LIST>");   break;
     case TOKEN_RUN:           return ("<CMD RUN>");    break;
     case TOKEN_UNSAVE:        return ("<CMD UNSAVE>"); break;
     case TOKEN_SYSTEM:        return ("<CMD SYSTEM>"); break;
     case TOKEN_BYE:           return ("<CMD BYE>");    break;
     case TOKEN_GOODBYE:       return ("<CMD GOODBYE>");break;
     case TOKEN_OK:            return ("<OK>");         break;
     default:                  return ("????");         break;
  }
}

/**
 * @brief     Look for any keywords or any direct commands
 * @fn        Token_t TokenKeyword (char *Bufferp) 
 * @param[in] **Bufferp - Buffer to tokenize
 * @return    Token_t 
 * @details   Is this is Key word (Language or direct)
 * @note
 * @todo     
 */
Token_t TokenDirectKeyword (char *Bufferp) {
  TokenCommandList_t *pRow;

  pRow = (TokenCommandList_t *)&TokenKeyword[0];  
  while (pRow->cmdstr != NULL) {
    if (StringMatch(Bufferp,pRow->cmdstr)) {
       return pRow->TokenValue;
    }
    pRow++;
  }    
 
  return pRow->TokenValue;
}

/**
 * @brief     Look for any direct commands
 * @fn        Token_t TokenDirectCommand (char *Bufferp) 
 * @param[in] **Bufferp - Buffer to tokenize
 * @return    Token_t 
 * @details   Is this is direct word 
 * @note
 * @todo     
 */
Token_t TokenDirectCommand (char *Bufferp) {
  TokenCommandList_t *pRow;

  pRow = (TokenCommandList_t *)&TokenCommand[0];  
  while (pRow->cmdstr != NULL) {
    if (StringMatch(Bufferp,pRow->cmdstr)) {
       return pRow->TokenValue;
    }
    pRow++;
  }    
 
  return pRow->TokenValue;
}

/**
 * @brief     Process Number Token
 * @fn        Token_t TokenGetNumber(char *Bufferp, char *Tokenp) 
 * @param[in] **Bufferp - Buffer to tokenize
 * @param[out]*Tokenp  - add to tokenized buffer
 * @return    Token_t 
 * @details   Builds the value of the number as a literal integer and not ascii. 
 * @note
 * @todo      Floating point e.g. 2.56, -.25 .25
 *            Exponent       e.g. 2E10 or 2^10(?)
 */
Token_t TokenGetNumber(char **Bufferp, char *Tokenp, Token_t PreToken) {
  char *Bufp;
  uint32_t DigitCount = 0;
  bool isFloatingPoint = false;  
  float power = 1.0;
  float value = 0.0;
  
  Bufp = *Bufferp;  

  if (Verbose) printf("TokenGetNumber %c \n", *Bufp);
#if 0
  do {
    value = 10 * value + (*Bufp -'0');
    *Tokenp++ = *Bufp++;
    DigitCount++;
  } while ( (isdigit(*Bufp)) && (DigitCount < MAX_DIGIT_COUNT));
#else
  while ( (isdigit(*Bufp) && (DigitCount < MAX_DIGIT_COUNT)) && (PreToken != TOKEN_PERIOD)) {
    if (Verbose) printf("TokenGetNumber: pre-floating point %f\n", value);    
    value = 10 * value + (*Bufp -'0');
    *Tokenp++ = *Bufp++;
    DigitCount++;
  } 
#endif  
  if (DigitCount >= MAX_DIGIT_COUNT) {
    Error("%s", ErrorToString(ERROR_NUMBER_TOO_LARGE));

    return TOKEN_ERROR;
  }

  if (*Bufp == '.' || PreToken == TOKEN_PERIOD ) {      /* This could be a floating point number */
    if (Verbose) printf("TokenGetNumber: floating point %f\n", value);
    Bufp++;
    while ((isdigit(*Bufp)) && (DigitCount < MAX_DIGIT_COUNT)) {
      value = 10.0 * value + (*Bufp -'0');
      power *= 10.0;
      *Tokenp++ = *Bufp++;
      DigitCount++;
      if (Verbose) printf("TokenGetNumber: %f\n", value);      
    }
    isFloatingPoint = true;
  }
  
  if (isFloatingPoint) {
    value = value / power;
    Literal.Type  = LITERAL_FLOAT;
    Literal.value.FloatValue = (PreToken == TOKEN_MINUS) ? -value : value;
  } else {
    Literal.Type  = LITERAL_INTEGER;
    Literal.value.IntegerValue = (PreToken == TOKEN_MINUS) ? -value : value;
  }
  
  *Tokenp = '\0';
  *Bufferp = (char *)Bufp;
    
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
  char *t = Tokenp;
  Token_t TokenReturn = TOKEN_OK;
  
  Bufp = *Bufferp;
  
  if (Verbose) printf("TokenGetWord %c\n", (int)*Bufp);
  
  while ( ((isalnum(*Bufp)) || (*Bufp != '\0')) && (!isspace(*Bufp)) ) {
      *Tokenp++ = *Bufp++;
  }
  *Tokenp = '\0';
  //  printf("Last [%p] = %d\n", (void *)Tokenp, *Tokenp);
  
  *Bufferp = (char *)Bufp;

  /*
   * Test if this s Keyword, TOKEN_ERROR means it isnt
   */
  TokenReturn = TokenDirectKeyword(t);

  return TokenReturn;
}
  
/**
 * @brief     Process string (alpha) token
 * @fn        Token_t TokenGetString  (char *Bufferp, char *Tokenp) 
 * @param[in] Bufferp - Buffer to tokenize
 * @param[out]Tokenp  - add to tokenized buffer
 * @return    Token_t 
 * @notes     Tokenizer ceases when a SPACE or end of line if found
 * @details
 * @todo
 */
Token_t TokenGetString (char **Bufferp, char *Tokenp) {
  char *Bufp;

  Bufp = *Bufferp;
  
  if (Verbose) printf("TokenGetString %c\n", (int)*Bufp);
  
  while ( ((isalnum(*Bufp)) || (*Bufp != '\0')) && (!isspace(*Bufp)) ) {
      *Tokenp++ = *Bufp++;
      if (Verbose) printf("[%p] %c = %c\n", (void *)Tokenp, *Tokenp, *Bufp);            
  }
  *Tokenp = '\0';

  Literal.Type  = LITERAL_STRING;  
  strcpy(Literal.value.StringValue, Tokenp);

  *Bufferp = (char *)Bufp;
  
  return TOKEN_STRING;
}

/**
 * @brief      Process Special Token
 * @fn         Token_t TokenGetSpecial(char *Bufferp, char *Tokenp) 
 * @param[in]  Bufferp - Buffer pointer to input characters. Pointer is moved and passed back.
 * @param[out] Tokenp  - add to token buffer for further processing. 
 * @return     Token_t - The type of tokem found
 * @notes      The Special tokens are the non Alpha numerics. 
 *             Logical operators need extra handling e.g. >= <= <> requires the next character be parsed as well
 *             >  GT
 *             <  LT
 *             <> NE
 *             >= GE
 *             <= LE
 */
Token_t TokenGetSpecial(char **Bufferp, char *Tokenp) {
    char *Bufp;
    Token_t TokenReturn;
    
    Bufp = *Bufferp;

    if (Verbose) printf("TokenGetSpecial %c\n", *Bufp);

    *Tokenp++ = *Bufp;     /* Copy input buffer character to the Token Buffer */
    
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
      case ',': TokenReturn = TOKEN_COMMA;         break;
      case '\\':TokenReturn = TOKEN_BACK_SLASH;    break;
      case '"': TokenReturn = TOKEN_QUOTE;         break;
      case '\'':TokenReturn = TOKEN_SINGLE_QUOTE;  break;
      case '`': TokenReturn = TOKEN_OPEN_QUOTE;    break;        
      case '.': TokenReturn = TOKEN_PERIOD;        break;
      case ':': TokenReturn = TOKEN_COLON;         break;
      case ';': TokenReturn = TOKEN_SEMI_COLON;    break;                
      case '?': TokenReturn = TOKEN_QUESTION_MARK; break;
      case '<': {                           /* < <> <= */
        if (*(Bufp+1) == '=') {
          TokenReturn = TOKEN_LE;
          Bufp++;
	  *Tokenp++ = '=';
        } else if (*(Bufp+1) == '>') {
          TokenReturn = TOKEN_NE;
          Bufp++;
	  *Tokenp++ = '>';
        } else {
          TokenReturn = TOKEN_LT;
        }
        break;
      }
      case '>': {                           /* > >= */
        if (*(Bufp+1) == '=') {
          TokenReturn = TOKEN_GE;
          Bufp++;
	  *Tokenp++ = '=';
        } else {
          TokenReturn = TOKEN_GT;
        }
        break;
      }
      default:  TokenReturn = TOKEN_ERROR;
    }

    Bufp++;
    *Tokenp = '\0';

    *Bufferp = Bufp;    /* Return last position in the input buffer */
    
    return TokenReturn;    
}

/**
 * @brief      Process direct commands
 * @fn         Token_t TokenGetDirect(char **Bufferp, char *Tokenp) 
 * @param[in]  Bufferp - Buffer to tokenize
 * @param[out] Tokenp  - add to tokenized buffer
 * @return     Token_t 
 * @notes     
 */
Token_t TokenGetDirect(char **Bufferp, char *Tokenp) {
    char *Bufp;

    if (Verbose) printf("TokenGetDirect %s\n", Tokenp);
    
    Bufp = *Bufferp;
    *Tokenp++ = *Bufp;

    return TOKEN_ERROR;
}

/**
 * @brief     Print the token string
 * @fn        Token_t TokenGetWord  (char *Bufferp, char *Tokenp) 
 * @param[in] *TokenString - actual token buffer
 * @param[in] Token        - The token found
 * @return    void
 * @notes     
 * @details   Some tokens (SPACE) are ignored
 * @todo
 */
void TokenPrint (char *TokenString, Token_t Token) {

    if (*TokenString != '\0') {
      char *Return;

      printf("\t> %16s   %16s", TokenGetStringType(Token), TokenString);
      if (Token == TOKEN_DIGIT ) {
	if (Literal.Type == LITERAL_INTEGER) {
	  printf("   INTEGER = %16d", Literal.value.IntegerValue);
	} else if (Literal.Type == LITERAL_FLOAT) {
	  printf("   FLOAT   = %16f", Literal.value.FloatValue);	  
	} else {
	  printf("   <UNKNOWN LITERAL?>\n");	  	  
	}
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
 * @note      Reads a line of text until EOF
 *            For each line, spaces are skipped and all the characters are parsed until EOL (or ERROR)
 *            - Each item (DIGIT, WORD, SPECIAL
 *            The TokenBuffer contains the parsed line elements now in tokens
 */
int32_t Tokenize (char *FileName) {
  FILE *fp;
  int32_t ErrorCode = SUCCESS;
  char *Bufferp;
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

  /*
   * Read the file until the EOF
   */
  while (UtilsReadSourceLine(fp, Bufferp) == true) {
    Bufferp = UtilsSkipSpaces(Bufferp);
     
     printf(">> %s", Bufferp);

     while (*Bufferp != '\0' && Token != TOKEN_ERROR) {     /* Test for Numbers including -ve ones      */
       if ((Token == TOKEN_MINUS && isdigit(*Bufferp+1)) ||
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

       TokenPrint(TokenBuffer, Token);                      /* Show the Token buffer contentst           */
              
       memset(TokenBuffer, '\0', sizeof(TokenBuffer));      /* Clear Token buffer on each line parse     */
     }
  }
  Token = TOKEN_EOF;
  TokenPrint(TokenBuffer, Token);  
  
  return ErrorCode;
}
