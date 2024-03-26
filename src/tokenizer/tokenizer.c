/**
 *****************************************************************************
 * @brief   basic interpreter - tokenizer
 * @author  onyettr
 * @file    tokenizer.c
 *
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
extern SymbolTableNode_t *symTable;

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
static char TokenBuffer [MAX_SOURCE_LINE_LENGTH+10];   /*!> All tokens after parsing each line */
static char SourceBuffer[MAX_SOURCE_LINE_LENGTH+10];   /*!> Line read from the file            */
static Literal_t Literal;

/**
 *  @brief  All Direct (not keywords) commands
 *  @struct TokenCommandList_t
 *  @note   Not part of the BASIC dialect, but commands used for the 
 *          DTSS (Dartmouth Timesharing System)
 */
static TokenCommandList_t TokenDirectCommandList[] = {
    { "HELLO"   , "<sign on>    " , TOKEN_HELLO     , DTSSCommandHello  },
    { "NEW"     , "new program  " , TOKEN_NEW       , DTSSCommandNew    },
    { "OLD"     , "last program " , TOKEN_OLD       , DTSSCommandOld    },
    { "SAVE"    , "save current " , TOKEN_SAVE      , DTSSCommandSave   },
    { "REPLACE" , "overwrite    " , TOKEN_REPLACE   , DTSSCommandReplace},
    { "RENAME"  , "replace      " , TOKEN_RENAME    , DTSSCommandRename },
    { "CAT"     , "list         " , TOKEN_CAT       , DTSSCommandCat    },
    { "LIST"    , "list         " , TOKEN_LIST      , DTSSCommandList   },
    { "RUN"     , "execute      " , TOKEN_RUN       , DTSSCommandRun    },
    { "STOP"    , "stop         " , TOKEN_CMD_STOP  , DTSSCommandStop   },
    { "UNSAVE"  , "undo         " , TOKEN_UNSAVE    , DTSSCommandUnsave },
    { "SYSTEM"  , "system cmd   " , TOKEN_SYSTEM    , DTSSCommandSystem },
    { "BYE"     , "logoff       " , TOKEN_BYE       , NULL              },
    { "GOODBYE" , "really logoff" , TOKEN_GOODBYE   , NULL              },
    { "SCRATCH" , "new          " , TOKEN_SCRATCH   , DTSSCommandScratch},
    { "FRI"     , "friden mode  " , TOKEN_FRI       , NULL              },
    { "NFR"     , "exit friden  " , TOKEN_NFR       , NULL              },
    { "EXPLAIN" , "help         " , TOKEN_EXPLAIN   , DTSSCommandExplain},
    { "SYMLIST" , "DBG symtable " , TOKEN_SYMTABLE_LIST, TT_SymbolTableShow},
    { "HELP"    , "help         " , TOKEN_HELP      , TT_InteractiveHelp},
    { NULL      , NULL            , TOKEN_WORD      , NULL              }    
};

/**
 *  @brief  All language reserved Keywords
 *  @struct TokenCommandList_t 
 */
static TokenCommandList_t TokenKeywordList[] = {
    { "LET"     , NULL, TOKEN_LET     , NULL },
    { "PRINT"   , NULL, TOKEN_PRINT   , NULL },
    { "END"     , NULL, TOKEN_END     , NULL },
    { "READ"    , NULL, TOKEN_READ    , NULL },
    { "DATA"    , NULL, TOKEN_DATA    , NULL },
    { "GOTO"    , NULL, TOKEN_GOTO    , NULL },
    { "IF"      , NULL, TOKEN_IF      , NULL },
    { "THEN"    , NULL, TOKEN_THEN    , NULL },    
    { "FOR"     , NULL, TOKEN_FOR     , NULL },
    { "TO"      , NULL, TOKEN_TO      , NULL },    
    { "NEXT"    , NULL, TOKEN_NEXT    , NULL },
    { "GOSUB"   , NULL, TOKEN_GOSUB   , NULL },
    { "RETURN"  , NULL, TOKEN_RETURN  , NULL },
    { "DEF"     , NULL, TOKEN_DEF     , NULL },
    { "DIM"     , NULL, TOKEN_DIM     , NULL },    
    { "REM"     , NULL, TOKEN_REM     , NULL },
    { "MAT"     , NULL, TOKEN_MAT     , NULL },      /* 2nd edition */
    { "SGN"     , NULL, TOKEN_SGN     , NULL },      /* 3rd edition */
    { "RESTORE" , NULL, TOKEN_RESTORE , NULL },
    { "INPUT"   , NULL, TOKEN_INPUT   , NULL },
    { "RANDOMIZE",NULL, TOKEN_RANDOMIZE,NULL },      /* 4th edition */
    { "ON"      , NULL, TOKEN_ON      , NULL },
    { "CHANGE"  , NULL, TOKEN_CHANGE  , NULL },
    { "RESET"   , NULL, TOKEN_RESET   , NULL },      /* 5th edition */
    { "LOC"     , NULL, TOKEN_LOC     , NULL },
    { "LOF"     , NULL, TOKEN_LOF     , NULL },
    { "CHAIN"   , NULL, TOKEN_CHAIN   , NULL },
    { "COMMON"  , NULL, TOKEN_COMMON  , NULL },
    { NULL      , NULL, TOKEN_WORD    , NULL }    
};

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

/**
 * @brief     Match strings
 * @fn        static bool StringMatch(char *str1, char *str2)
 * @param[in] *str1 - first string
 * @param[in] *str2 - to match to
 * @return    true if str1 matches str2, false otherwise
 * @details   
 * @note
 * @todo     
 */
static bool StringMatch(char *str1,char *str2) {
  int i;

  UtilsToUpper(str1);        /* Convert to all upper case */

  for (i=0; i < (int)strlen(str2); i++) {
    if (str1[i] != str2[i] ) {
      return false;           /* No match                 */
    }
  }

  //  printf("Match cmp %s with %s\n", str1, str2);
  return(!(strncmp(str1,str2, strlen(str1))));
}

int32_t TT_InteractiveHelp(void) {
  int32_t ErrorCode = SUCCESS;
  TokenCommandList_t *pRow;

  pRow = (TokenCommandList_t *)&TokenDirectCommandList[0]; 
  while (pRow->cmdstr != NULL) {
    printf("[%8s] %10s %s\n",
	   pRow->cmdstr,
	   pRow->HelpString,
	   pRow->pDirectFunction == NULL ? "<empty>" : "<full>");
	   
    pRow++;
  }

  return ErrorCode;
}

/**
 * @brief     Token types to string convert
 * @fn        char *TokenGetStringType(Token_t Token)
 * @param[in] Token - to switch on
 * @return    char* string - token as a string
 * @note
 */
char *TokenGetStringType(Token_t Token) {

  switch (Token) {
     case TOKEN_WORD:          return ("<WORD>");       break;
     case TOKEN_STRING:        return ("<STRING>");     break;
     case TOKEN_DIGIT:         return ("<DIGIT>");      break;       
     case TOKEN_LETTER:        return ("<LETTER>");     break;
     case TOKEN_IDENTIFIER:    return ("<IDENTIFIER>"); break;
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
     case TOKEN_SCRATCH:       return ("<CMD SCRATCH>");break;
     case TOKEN_EXPLAIN:       return ("<CMD EXPLAIN>");break;
     case TOKEN_BYE:           return ("<CMD BYE>");    break;
     case TOKEN_HELP:          return ("<DBG HELP>");   break;
     case TOKEN_SYMTABLE_LIST: return ("<DBG SYMLIST>");break;
     case TOKEN_GOODBYE:       return ("<CMD GOODBYE>");break;
     case TOKEN_OK:            return ("<OK>");         break;
     default:                  return ("????");         break;
  }
}

/**
 * @brief     Look for any keywords or any direct commands
 * @fn        Token_t TokenDirectKeyword (char *Bufferp) 
 * @param[in] *Bufferp - Buffer to tokenize
 * @return    Token_t 
 * @details   Scan all Direct keywords to see of this is Key word
 * @note      List is NULL terminated, stop when we reach here
 * @todo     
 */
Token_t TokenDirectKeyword (char *Bufferp) {
  TokenCommandList_t *pRow;
  
  pRow = (TokenCommandList_t *)&TokenKeywordList[0];  
  while (pRow->cmdstr != NULL) {
    if (StringMatch(Bufferp,pRow->cmdstr)) {
       return pRow->TokenValue;
    }

    pRow++;
  }    
 
  return pRow->TokenValue;
}

/**
 * @brief     Look for any direct commands in supplied buffer
 * @fn        Token_t TokenDirectCommand (char *Bufferp) 
 * @param[in] *Bufferp - Buffer to tokenize
 * @return    Token_t 
 * @details   Is this is direct word?
 * @note
 * @todo      This should be combined with TokenDirectKeyword (or same array?)
 */
Token_t TokenDirectCommand (char *Bufferp) {
  TokenCommandList_t *pRow;
  
  pRow = (TokenCommandList_t *)&TokenDirectCommandList[0];  
  while (pRow->cmdstr != NULL) {
    if (StringMatch(Bufferp,pRow->cmdstr)) {    
       return pRow->TokenValue;
    }
    pRow++;
  }    
 
  return pRow->TokenValue;
}

/**
 * @brief     is this a direct command?
 * @fn        bool IsTokenDirectCommand (char *Bufferp) 
 * @param[in] *Bufferp - Buffer to tokenize
 * @return    true if a direct command, false otherwise
 * @details   Is this is direct word?
 * @note
 * @todo    
 */
bool IsTokenDirectCommand (char *Bufferp) {
  TokenCommandList_t *pRow;
  
  pRow = (TokenCommandList_t *)&TokenDirectCommandList[0];  
  while (pRow->cmdstr != NULL) {
    if (StringMatch(Bufferp,pRow->cmdstr)) {    
       return true;
    }
    pRow++;
  }    
 
  return false;
}

/**
 * @brief     Is this a Keyword?
 * @fn        bool IsTokenDirectKeyword (char *Bufferp) 
 * @param[in] *Bufferp - Buffer to tokenize
 * @return    true if a Keyword, false otherwise
 * @details  
 * @note      List is NULL terminated, stop when we reach here
 * @todo     
 */
bool IsTokenDirectKeyword (char *Bufferp) {
  TokenCommandList_t *pRow;
  
  pRow = (TokenCommandList_t *)&TokenKeywordList[0];  
  while (pRow->cmdstr != NULL) {
    if (StringMatch(Bufferp,pRow->cmdstr)) {
       return true;
    }

    pRow++;
  }    
 
  return false;
}

/**
 * @brief     Execute the direct aommand
 * @fn        int32_t TokenExecuteDirectCommand (Token_t CommandToken)
 * @param[in] CommandToken - execute this command
 * @param[in] TokenStr     - execute this command
 * @return    int32_t
 * @details   Execute the Direct command callback, if present
 * @note
 * @todo      This should be combined as we have to search again
 * @todo      should use int argc, char *argv[] for params to DirectFunction callback
 */
int32_t TokenExecuteDirectCommand (Token_t CommandToken, char *Tokenp) {
  int32_t ErrorCode = SUCCESS;
  TokenCommandList_t *pRow;

  pRow = (TokenCommandList_t *)&TokenDirectCommandList[0];  
  while (pRow->cmdstr != NULL) {
    if (CommandToken == pRow->TokenValue) { /* Found a match for the CommandToken */
      if (pRow->pDirectFunction != NULL) {  
        (pRow->pDirectFunction)();               /* Execute the command callback       */
      }
    }
    pRow++;
  }    

  UNUSED(Tokenp);
  
  return ErrorCode;
}

/**
 * @brief      Process Number Token
 * @fn         Token_t TokenGetNumber(char **Bufferp, char *Tokenp, Token_t PreToken) 
 * @param[in]  **Bufferp - Buffer to tokenize, updates the pointer on return
 * @param[out] *Tokenp   - tokenized buffer after parsing
 * @param[in]  PreToken  - any already processed Tokens e.g. TOKEN_MINUS
 * @return     Token_t 
 * @details    Builds the value of the number as a literal integer or float and not ascii. 
 * @note       Needs to support:
 * @verbatim
               integer values  e.g. 12, 199, 3
               negative values e.g. -1 -199, -3
               Floating point  e.g. 2.56, -.25 .25
               Exponent        e.g. 2E10 or 2^10(?)
 * @endverbatim
 * @todo       Exponent        e.g. 2E10 or 2^10(?)
 */
Token_t TokenGetNumber(char **Bufferp, char *Tokenp, Token_t PreToken) {
  uint32_t DigitCount = 0;  
  float power = 1.0;
  float value = 0.0;
  char *Bufp;  
  bool isFloatingPoint = false;
  bool isDot = false;
  
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
    Error("TokenGetNumber %s", ErrorToString(ERROR_NUMBER_TOO_LARGE));

    return TOKEN_ERROR;
  }

  if (*Bufp == '.' || PreToken == TOKEN_PERIOD ) {      /* This could be a floating point number */
    if (Verbose) printf("TokenGetNumber: floating point %f\n", value);

    *Tokenp++ = *Bufp++;

    while ((isdigit(*Bufp)) && (DigitCount < MAX_DIGIT_COUNT)) {
      value = 10.0 * value + (*Bufp -'0');
      power *= 10.0;
      *Tokenp++ = *Bufp++;
      DigitCount++;
      if (Verbose) printf("TokenGetNumber: %f\n", value);      
    }
    isFloatingPoint = true;
    isDot = true;
  }

  if (DigitCount >= MAX_DIGIT_COUNT) {
    Error("TokenGetNumber %s", ErrorToString(ERROR_NUMBER_TOO_LARGE));

    return TOKEN_ERROR;
  }

  /*
   * Is this Exponent format?
   * 
   */
  Bufp = UtilsSkipSpaces(Bufp);           /* Skip any leading spaces */
  //  if ((!isDot) && (*Bufp == 'E' || *Bufp == 'e')) {
  if ((*Bufp == 'E' || *Bufp == 'e')) {
    float evalue = 0.0;
    
    if (Verbose) printf("TokenGetNumber: Exponent seen\n");

    *Tokenp++ = *Bufp++;

    Bufp = UtilsSkipSpaces(Bufp);    
    while ((isdigit(*Bufp)) && (DigitCount < MAX_DIGIT_COUNT)) {
      evalue = 10.0 * evalue + (*Bufp -'0');
      power *= 10.0;
      *Tokenp++ = *Bufp++;
      DigitCount++;

      if (Verbose) printf("TokenGetNumber: Exponent = %f\n", evalue);      
    }

    Bufp = UtilsSkipSpaces(Bufp);
    value *= pow(10, evalue);
    power = 1.0;
    if (Verbose) printf("TokenGetNumber: E = %f\n", value);          
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
    
  UNUSED(isDot);
  
  return TOKEN_DIGIT;
}

/**
 * @brief      Process word (alpha) token
 * @fn         Token_t TokenGetWord  (char **Bufferp, char *Tokenp) 
 * @param[in]  **Bufferp - Buffer to tokenize
 * @param[out] *Tokenp   - add to tokenized buffer
 * @return     Token_t 
 * @note       Tokenizer ceases when a SPACE or end of line is found
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
  
  *Bufferp = (char *)Bufp;

  /*
   * Test if this s Keyword, TOKEN_ERROR means it isn't
   */
  TokenReturn = TokenDirectKeyword(t);

  return TokenReturn;
}
  
/**
 * @brief      Process string (alpha) token, string denoted by a quote
 * @fn         Token_t TokenGetString  (char **Bufferp, char *Tokenp)
 * @param[in]  **Bufferp - Buffer to tokenize 
 * @param[out] *Tokenp   - add to tokenized buffer
 * @return     Token_t 
 * @note       Tokenizer ceases when a SPACE or end of line if found
 * @details    Creates a LITERAL (string) 
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
 * @fn         Token_t TokenGetSpecial(char **Bufferp, char *Tokenp) 
 * @param[in]  **Bufferp - Buffer pointer to input characters. Pointer is moved and passed back.
 * @param[out] *Tokenp   - add to token buffer for further processing. 
 * @return     Token_t - The type of tokem found
 * @note       The Special tokens are the non Alpha numerics. 
 * @verbatim
               Logical operators need extra handling e.g. >= <= <> requires the next character be parsed as well
               >  GT
               <  LT
               <> NE
               >= GE
               <= LE
   @endverbatim
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
 * @param[in]  **Bufferp - Buffer to tokenize
 * @param[out] *Tokenp   - add to tokenized buffer
 * @return     Token_t 
 * @note       None
 */
Token_t TokenGetDirect(char **Bufferp, char *Tokenp) {
    char *Bufp;

    if (Verbose) printf("TokenGetDirect %s\n", Tokenp);
    
    Bufp = *Bufferp;
    *Tokenp++ = *Bufp;

    return TOKEN_ERROR;
}

/**
 * @brief     Print the token string, includes the original line plus the Token as a string
 * @fn        void TokenPrint (char *TokenString, Token_t Token) 
 * @param[in] *TokenString - actual token buffer
 * @param[in] Token        - The token
 * @return    void
 * @note      
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
	  printf("   FLOAT   = %f", Literal.value.FloatValue);	  
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
 * @param[in] *FileName - file containing lines to Tokenize
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
  Token_t Token = TOKEN_NO_TOKEN;
  SymbolTableNode_t *pNewNode = NULL;
  
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

       if (Token == TOKEN_WORD) {                           
	 if (IsTokenDirectCommand(TokenBuffer)) {           /* Test for Direct Command                  */
           Token = TokenDirectCommand(TokenBuffer);         /* Which direct command?                    */
           TokenExecuteDirectCommand(Token, TokenBuffer);   /* Execute direct command                   */
	 } else if (IsTokenDirectKeyword(TokenBuffer)) {    /* Test for a Keyword                       */
	   printf("TODO Keyword...\n");
	 } else {                                           /* This is an identifier                    */
	   pNewNode = symbol_table_search(TokenBuffer, symTable);
	   if (pNewNode == NULL) {
	     pNewNode = symbol_table_add_name(TokenBuffer, &symTable);
	   }

	   Token = TOKEN_IDENTIFIER;
	 }
       }
       
       TokenPrint(TokenBuffer, Token);                      /* Show the Token buffer contentst           */
              
       memset(TokenBuffer, '\0', sizeof(TokenBuffer));      /* Clear Token buffer on each line parse     */
     }
  }
  Token = TOKEN_EOF;
  TokenPrint(TokenBuffer, Token);  

  if (symTable != NULL) {
    aymbol_table_clean(symTable);
  }

  fclose(fp);
  
  return ErrorCode;
}
