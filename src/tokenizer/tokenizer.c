/**
 *****************************************************************************
 * @brief   basic interpreter - tokenizer
 * @author  onyettr
 * @ingroup tokenizer tokenizer
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
#include "dtss.h"

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
    { "HELLO"   , "<sign on>    " , TOKEN_HELLO     , DTSS_command_hello    },
    { "NEW"     , "new program  " , TOKEN_NEW       , DTSSCommandNew        },
    { "OLD"     , "last program " , TOKEN_OLD       , DTSSCommandOld        },
    { "SAVE"    , "save current " , TOKEN_SAVE      , DTSSCommandSave       },
    { "REPLACE" , "overwrite    " , TOKEN_REPLACE   , DTSSCommandReplace    },
    { "RENAME"  , "replace      " , TOKEN_RENAME    , DTSSCommandRename     },
    { "CAT"     , "list         " , TOKEN_CAT       , DTSSCommandCat        },
    { "LIST"    , "list         " , TOKEN_LIST      , DTSSCommandList       },
    { "RUN"     , "execute      " , TOKEN_RUN       , DTSSCommandRun        },
    { "STOP"    , "stop         " , TOKEN_CMD_STOP  , DTSSCommandStop       },
    { "UNSAVE"  , "undo         " , TOKEN_UNSAVE    , DTSSCommandUnsave     },
    { "SYSTEM"  , "system cmd   " , TOKEN_SYSTEM    , DTSSCommandSystem     },
    { "BYE"     , "logoff       " , TOKEN_BYE       , NULL                  },
    { "GOODBYE" , "really logoff" , TOKEN_GOODBYE   , NULL                  },
    { "SCRATCH" , "new          " , TOKEN_SCRATCH   , DTSSCommandScratch    },
    { "FRI"     , "friden mode  " , TOKEN_FRI       , NULL                  },
    { "NFR"     , "exit friden  " , TOKEN_NFR       , NULL                  },
    { "EXPLAIN" , "help         " , TOKEN_EXPLAIN   , DTSSCommandExplain    },
    { "SYMLIST" , "DBG symtable " , TOKEN_SYMTABLE_LIST, TT_SymbolTableShow },
    { "HELP"    , "help         " , TOKEN_HELP      , TT_InteractiveHelp    },
    { NULL      , NULL            , TOKEN_WORD      , NULL                  }
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
 * @fn        static bool string_match(char *str1, char *str2)
 * @param[in] *str1 - first string
 * @param[in] *str2 - to match to
 * @return    true if str1 matches str2, false otherwise
 * @details
 * @note
 * @todo
 */
static bool string_match(char *str1,char *str2) {
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

/**
 * @brief show the list of token commands
 *
 * @return
 */
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
 * @fn        char *TOKEN_type_to_string(Token_t Token)
 * @param[in] Token - to switch on
 * @return    char* string - token as a string
 * @note
 */
char *TOKEN_type_to_string(Token_t Token) {

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
Token_t TOKEN_direct_keyword (char *buffer_p) {
  TokenCommandList_t *pRow;
  
  pRow = (TokenCommandList_t *)&TokenKeywordList[0];  
  while (pRow->cmdstr != NULL) {
    if (string_match(buffer_p,pRow->cmdstr)) {
       return pRow->TokenValue;
    }

    pRow++;
  }    
 
  return pRow->TokenValue;
}

/**
 * @brief     Look for any direct commands in supplied buffer
 * @fn        Token_t TokenDirectCommand (char *Bufferp) 
 * @param[in] *buffer_pp - Buffer to tokenize
 * @return    Token_t 
 * @details   Is this is direct word?
 * @note
 * @todo      This should be combined with TokenDirectKeyword (or same array?)
 */
Token_t TOKEN_direct_command (char *buffer_p) {
  TokenCommandList_t *pRow;
  
  pRow = (TokenCommandList_t *)&TokenDirectCommandList[0];  
  while (pRow->cmdstr != NULL) {
    if (string_match(buffer_p,pRow->cmdstr)) {
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
bool is_direct_command (char *Bufferp) {
  TokenCommandList_t *pRow;
  
  pRow = (TokenCommandList_t *)&TokenDirectCommandList[0];  
  while (pRow->cmdstr != NULL) {
    if (string_match(Bufferp,pRow->cmdstr)) {
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
bool is_direct_keyword (char *Bufferp) {
  TokenCommandList_t *pRow;
  
  pRow = (TokenCommandList_t *)&TokenKeywordList[0];  
  while (pRow->cmdstr != NULL) {
    if (string_match(Bufferp,pRow->cmdstr)) {
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
int32_t TOKEN_execute_direct_command (Token_t CommandToken, char *Tokenp) {
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
Token_t TOKEN_get_number(char **Bufferp, char *Tokenp, Token_t PreToken) {
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
    Error("TokenGetNumber %s", error_to_string(ERROR_NUMBER_TOO_LARGE));

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
    Error("TokenGetNumber %s", error_to_string(ERROR_NUMBER_TOO_LARGE));

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
Token_t TOKEN_get_word(char **Bufferp, char *Tokenp) {
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
  TokenReturn = TOKEN_direct_keyword(t);

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
Token_t TOKEN_get_string(char **Bufferp, char *Tokenp) {
  char *Bufp;

  Bufp = *Bufferp;

  if (Verbose) {
	  printf("TokenGetString %c\n", (int)*Bufp);
  }

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
Token_t TOKEN_get_special(char **Bufferp, char *Tokenp) {
    char *Bufp;
    Token_t TokenReturn;

    Bufp = *Bufferp;

    if (Verbose)
    {
      printf("[DBG] TokenGetSpecial %c\n", *Bufp);
    }

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
      default:
    	  TokenReturn = TOKEN_ERROR;
    }

    if (Verbose)
    {
    	printf("[DBG] TokenGetSpecial %s\n", TOKEN_type_to_string(TokenReturn));
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
Token_t TOKEN_get_direct(char **Bufferp, char *Tokenp) {
    char *Bufp;

    if (Verbose) printf("TokenGetDirect %s\n", Tokenp);
    
    Bufp = *Bufferp;
    *Tokenp++ = *Bufp;

    return TOKEN_ERROR;
}

/**
 * @brief     Print the token string, includes the original line plus the Token
 * 			  as a string
 * @fn        void TokenPrint (char *TokenString, Token_t Token) 
 * @param[in] *TokenString - actual token buffer
 * @param[in] Token        - The token
 * @return    void
 * @note      
 * @details   Some tokens (SPACE) are ignored
 * @todo
 */
void TOKEN_print(char *TokenString, Token_t Token) {

    if (*TokenString != '\0') {
      char *Return;

      printf("\t> %16s   %16s", TOKEN_type_to_string(Token), TokenString);
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
               Token = TOKEN_get_number(&Bufferp, TokenBuffer, Token);
       } else if (isalnum(*Bufferp)) {                      /* Test for Numbers and Letters             */
         Token = TOKEN_get_word(&Bufferp, TokenBuffer);
       } else if (isspace(*Bufferp)) {                      /* Test for SPACE, we just skip             */
         Bufferp++;
	 Token = TOKEN_SPACE;
       } else if (*Bufferp == '"') {                        /* Test for STRINGS                         */
         Token = TOKEN_get_string(&Bufferp, TokenBuffer);
       } else if (*Bufferp == '\n' || *Bufferp == '\r') {   /* TODO: convert to spaces for TokenBuffer? */
         Bufferp++;
       } else {                                             /* Test for Special characters              */
         Token = TOKEN_get_special(&Bufferp, TokenBuffer);
       }

       if (Token == TOKEN_WORD) {                           
    	   if (is_direct_command(TokenBuffer)) {           /* Test for Direct Command                  */
              Token = TOKEN_direct_command(TokenBuffer);         /* Which direct command?                    */
              TOKEN_execute_direct_command(Token, TokenBuffer);   /* Execute direct command                   */
	 } else if (is_direct_keyword(TokenBuffer)) {    /* Test for a Keyword                       */
	   printf("TODO Keyword...\n");
	 } else {                                           /* This is an identifier                    */
	   pNewNode = symbol_table_search(TokenBuffer, symTable);
	   if (pNewNode == NULL) {
	     pNewNode = symbol_table_add_node(TokenBuffer, &symTable);
	   }

	   Token = TOKEN_IDENTIFIER;
	 }
       }
       
       TOKEN_print(TokenBuffer, Token);                      /* Show the Token buffer contentst           */
              
       memset(TokenBuffer, '\0', sizeof(TokenBuffer));      /* Clear Token buffer on each line parse     */
     }
  }
  Token = TOKEN_EOF;
  TOKEN_print(TokenBuffer, Token);

  if (symTable != NULL) {
    symbol_table_clean(symTable);
  }

  fclose(fp);
  
  return ErrorCode;
}

/**
 * @brief     Tokenize a given file with improved error handling and structure
 * @fn        int32_t Tokenize_v2 (char *FileName)
 * @param[in] *FileName - file containing lines to Tokenize
 * @return    int32_t ErrorCode
 * @note      Enhanced version with better separation of concerns and error handling
 */
int32_t Tokenize_v2(char *FileName) {
    FILE *fp = NULL;
    int32_t ErrorCode = SUCCESS;
    int32_t LineNumber = 0;

    // Input validation
    if (FileName == NULL || *FileName == '\0') {
        Error("No filename provided");
        return ERROR_FILE_NO_FILENAME;
    }

    // File operations
    fp = fopen(FileName, "r");
    if (fp == NULL) {
        Error("Failed to open %s", FileName);
        return ERROR_FILE_OPEN_FAILURE;
    }

    // Main processing loop
    while (UtilsReadSourceLine(fp, SourceBuffer)) {
        LineNumber++;
        ErrorCode = ProcessLine(SourceBuffer, LineNumber);

        if (ErrorCode != SUCCESS) {
            Error("Error processing line %d: %s", LineNumber, error_to_string(ErrorCode));
            // Continue processing or break based on error severity
            if (ErrorCode == ERROR_FATAL) {
                break;
            }
        }
    }

    // Cleanup
    if (fp != NULL) {
        fclose(fp);
    }

    // Final EOF token
    TOKEN_print("", TOKEN_EOF);

    // Symbol table cleanup
    if (symTable != NULL) {
        symbol_table_clean(symTable);
    }

    return ErrorCode;
}

/**
 * @brief     Process a single line of source code
 * @fn        int32_t ProcessLine(char *LineBuffer, int32_t LineNumber)
 * @param[in] *LineBuffer - source line to process
 * @param[in] LineNumber - current line number for error reporting
 * @return    int32_t ErrorCode
 */
int32_t ProcessLine(char *LineBuffer, int32_t LineNumber) {
    char *BufferPtr = NULL;
    Token_t CurrentToken = TOKEN_NO_TOKEN;
    int32_t ErrorCode = SUCCESS;
    int32_t TokenCount = 0;

    if (LineBuffer == NULL) {
        return ERROR_NULL_POINTER;
    }

    // Skip leading whitespace
    BufferPtr = UtilsSkipSpaces(LineBuffer);

    // Skip empty lines and comments
    if (*BufferPtr == '\0' || *BufferPtr == '\n') {
        return SUCCESS;
    }

    if (Verbose) {
        printf("Line %d: %s", LineNumber, BufferPtr);
    }

    // Process tokens in the line
    while (*BufferPtr != '\0' && ErrorCode == SUCCESS) {
        // Clear token buffer
        memset(TokenBuffer, '\0', sizeof(TokenBuffer));

        // Get next token
        CurrentToken = GetNextToken(&BufferPtr, TokenBuffer);

        if (CurrentToken == TOKEN_ERROR) {
            ErrorCode = ERROR_INVALID_TOKEN;
            Error("Invalid token at line %d, position %ld",
                  LineNumber, BufferPtr - LineBuffer);
            break;
        }

        // Skip whitespace tokens for cleaner output
        if (CurrentToken == TOKEN_SPACE) {
            continue;
        }

        // Process the token
        ErrorCode = ProcessToken(CurrentToken, TokenBuffer, LineNumber);

        if (Verbose || CurrentToken != TOKEN_SPACE) {
            TOKEN_print(TokenBuffer, CurrentToken);
        }

        TokenCount++;

        // Safety check for runaway tokenization
        if (TokenCount > MAX_TOKENS_PER_LINE) {
            Error("Too many tokens on line %d", LineNumber);
            ErrorCode = ERROR_TOO_MANY_TOKENS;
            break;
        }
    }

    return ErrorCode;
}

/**
 * @brief     Get the next token from the buffer
 * @fn        Token_t GetNextToken(char **BufferPtr, char *TokenBuffer)
 * @param[in/out] **BufferPtr - pointer to current position in buffer
 * @param[out] *TokenBuffer - buffer to store the token string
 * @return    Token_t - type of token found
 */
Token_t GetNextToken(char **BufferPtr, char *TokenBuffer) {
    char *Bufp = *BufferPtr;
    Token_t TokenType = TOKEN_NO_TOKEN;
    Token_t PreviousToken = TOKEN_NO_TOKEN;  // For context-sensitive parsing

    // Skip whitespace and update pointer
    while (isspace(*Bufp) && *Bufp != '\0') {
        if (*Bufp == ' ' || *Bufp == '\t') {
            Bufp++;
            *BufferPtr = Bufp;
            return TOKEN_SPACE;
        } else if (*Bufp == '\n' || *Bufp == '\r') {
            Bufp++;
            *BufferPtr = Bufp;
            return TOKEN_NO_TOKEN; // End of line
        }
        Bufp++;
    }

    // End of buffer
    if (*Bufp == '\0') {
        *BufferPtr = Bufp;
        return TOKEN_NO_TOKEN;
    }

    // Determine token type and parse accordingly
    if (IsNumericStart(Bufp, PreviousToken)) {
        TokenType = TOKEN_get_number(BufferPtr, TokenBuffer, PreviousToken);
    } else if (isalpha(*Bufp) || *Bufp == '_') {
        TokenType = TOKEN_get_word(BufferPtr, TokenBuffer);
    } else if (*Bufp == '"' || *Bufp == '\'') {
        TokenType = TOKEN_get_string(BufferPtr, TokenBuffer);
    } else {
        TokenType = TOKEN_get_special(BufferPtr, TokenBuffer);
    }

    return TokenType;
}

/**
 * @brief     Process a parsed token
 * @fn        int32_t ProcessToken(Token_t TokenType, char *TokenString, int32_t LineNumber)
 * @param[in] TokenType - type of token
 * @param[in] *TokenString - token string value
 * @param[in] LineNumber - current line number
 * @return    int32_t ErrorCode
 */
int32_t ProcessToken(Token_t TokenType, char *TokenString, int32_t LineNumber) {
    int32_t ErrorCode = SUCCESS;
    SymbolTableNode_t *pNewNode = NULL;

    switch (TokenType) {
        case TOKEN_WORD:
            if (is_direct_command(TokenString)) {
                TokenType = TOKEN_direct_command(TokenString);
                ErrorCode = TOKEN_execute_direct_command(TokenType, TokenString);
            } else if (is_direct_keyword(TokenString)) {
                // Handle keyword - could expand this for syntax analysis
                if (Verbose) {
                    printf("Keyword found: %s\n", TokenString);
                }
            } else {
                // Handle identifier
                pNewNode = symbol_table_search(TokenString, symTable);
                if (pNewNode == NULL) {
                    pNewNode = symbol_table_add_node(TokenString, &symTable);
                    if (pNewNode == NULL) {
                        Error("Failed to add symbol '%s' to table at line %d",
                              TokenString, LineNumber);
                        ErrorCode = ERROR_SYMBOL_TABLE_FULL;
                    }
                }
                TokenType = TOKEN_IDENTIFIER;
            }
            break;

        case TOKEN_DIGIT:
            // Could add range checking or literal processing here
            break;

        case TOKEN_STRING:
            // Could add string processing/validation here
            break;

        case TOKEN_ERROR:
            ErrorCode = ERROR_INVALID_TOKEN;
            break;

        default:
            // Most tokens need no special processing
            break;
    }

    return ErrorCode;
}

/**
 * @brief     Check if current position starts a numeric token
 * @fn        bool IsNumericStart(char *BufferPtr, Token_t PreviousToken)
 * @param[in] *BufferPtr - current buffer position
 * @param[in] PreviousToken - previously parsed token for context
 * @return    bool - true if this starts a number
 */
bool IsNumericStart(char *BufferPtr, Token_t PreviousToken) {
    if (isdigit(*BufferPtr)) {
        return true;
    }

    // Handle negative numbers and decimal points
    if ((*BufferPtr == '-' || *BufferPtr == '.') && isdigit(*(BufferPtr + 1))) {
        return true;
    }

    // Handle cases like "5." or ".5"
    if (*BufferPtr == '.' && (PreviousToken == TOKEN_DIGIT || isdigit(*(BufferPtr + 1)))) {
        return true;
    }

    return false;
}

// Additional utility functions for better error handling and validation

/**
 * @brief     Validate token buffer bounds
 * @fn        bool ValidateTokenBuffer(const char *TokenBuffer)
 * @param[in] *TokenBuffer - token buffer to validate
 * @return    bool - true if valid
 */
bool ValidateTokenBuffer(const char *TokenBuffer) {
    if (TokenBuffer == NULL) {
        return false;
    }

    size_t length = strlen(TokenBuffer);
    if (length >= MAX_SOURCE_LINE_LENGTH) {
        Error("Token too long: %zu characters", length);
        return false;
    }

    return true;
}

/**
 * @brief     Enhanced error reporting with context
 * @fn        void ReportTokenError(const char *Message, int32_t LineNumber, const char *Context)
 * @param[in] *Message - error message
 * @param[in] LineNumber - line where error occurred
 * @param[in] *Context - surrounding context
 */
void ReportTokenError(const char *Message, int32_t LineNumber, const char *Context) {
    Error("Tokenization error at line %d: %s", LineNumber, Message);
    if (Context != NULL && Verbose) {
        Error("Context: %s", Context);
    }
}
