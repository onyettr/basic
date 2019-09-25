/**
 *****************************************************************************
 * @brief   basic interpreter
 * @author  onyettr
 * @file    tokenizer.h
 ***************************************************************************** 
 */
#ifndef __TOKENIZER__H__
#define __TOKENIZER__H__

/*
******************************************************************************
Includes
******************************************************************************
*/

/*
******************************************************************************
Private Types
*****************************************************************************
*/

/**
 * @brief TOKEN values for the various elements
 * @enum  TokenValues_t
 */
typedef enum {
    TOKEN_WORD,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPECIAL,
    TOKEN_NO_TOKEN,
    TOKEN_TILDE,
    TOKEN_OPEN_QUOTE,
    TOKEN_PLING,
    TOKEN_AT,
    TOKEN_HASH,
    TOKEN_PERCENT,
    TOKEN_HAT,
    TOKEN_AMPERSAND,
    TOKEN_MULTIPLY,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_EQUAL,
    TOKEN_DASH,
    TOKEN_L_BRACKET,
    TOKEN_R_BRACKET,
    TOKEN_L_CURLY,
    TOKEN_R_CURLY,
    TOKEN_VERTICAL_BAR,
    TOKEN_BACK_SLASH,
    TOKEN_DIVIDE,
    TOKEN_LT,
    TOKEN_LE,    
    TOKEN_GT,
    TOKEN_GE,
    TOKEN_NE,        
    TOKEN_COMMA,
    TOKEN_SEMI_COLON,
    TOKEN_COLON,
    TOKEN_QUOTE,
    TOKEN_SINGLE_QUOTE,
    TOKEN_PERIOD,
    TOKEN_QUESTION_MARK,
    TOKEN_SPACE,
    
    /*
     * Reserved words tokens, based on Dartmouth BASIC original dialect
     */
    TOKEN_LET,
    TOKEN_PRINT,
    TOKEN_END,
    TOKEN_READ,
    TOKEN_DATA,
    TOKEN_GOTO,
    TOKEN_IF,
    TOKEN_FOR,
    TOKEN_NEXT,
    TOKEN_GOSUB,
    TOKEN_RETURN,
    TOKEN_DEF,
    TOKEN_DIM,
    TOKEN_REM,
    TOKEN_STOP,

    /*
     * Numeric functions
     */
    TOKEN_ABS,
    TOKEN_ATN,
    TOKEN_COS,
    TOKEN_EXP,
    TOKEN_INT,
    TOKEN_LOG,
    TOKEN_RND,
    TOKEN_SIN,
    TOKEN_SQR,
    TOKEN_TAN,

    /*
     * Direct commands
     */
    TOKEN_HELLO,
    TOKEN_NEW,
    TOKEN_OLD,
    TOKEN_SAVE,
    TOKEN_REPLACE,
    TOKEN_RENAME,
    TOKEN_CAT,
    TOKEN_LIST,
    TOKEN_RUN,
    TOKEN_CMD_STOP,
    TOKEN_UNSAVE,
    TOKEN_SYSTEM,
    TOKEN_BYE,
    TOKEN_GOODBYE,
    
    TOKEN_EOF,
    TOKEN_ERROR
} TokenValues_t;

/**
 *  @brief Token_t enumeration for Tokens
 *  
 */
typedef int32_t Token_t;

/**
 * @brief Literal Types string and integer
 * @enum LiteralType_t
 */
typedef enum {
   LITERAL_STRING,
   LITERAL_INTEGER
} LiteralType_t;

/**
 * @brief Literal string and integer storage
 * @struct Literal_t 
 */
typedef struct {
  LiteralType_t Type;
  union {
    int32_t IntegerValue;
    char    StringValue[MAX_SOURCE_LINE_LENGTH];
  } value;                   /*! union value */
} Literal_t;

typedef struct {
  char *cmdstr;
  Token_t TokenValue;
  //  int (*pDirectFunction)(int argc, char *argv[]);
  int (*pDirectFunction)(void);     /*! For future use to add callbacks for Keywords */
} TokenCommandList_t;

/*
******************************************************************************
Private variables (static)
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
int32_t Tokenize (char *FileName);
Token_t TokenGetNumber(char **Bufferp, char *Tokenp);
Token_t TokenGetWord  (char **Bufferp, char *Tokenp);
Token_t TokenGetSpecial(char **Bufferp, char *Tokenp);
Token_t TokenGetDirect(char **Bufferp, char *Tokenp);
char    *TokenGetStringType(Token_t Token);
void   TokenPrint (char *TokenString, Token_t Token);

#endif  /* __TOKENIZER_H__ */
