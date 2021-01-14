/**
 *******************************************************************************
 * @brief   basic interpreter
 * @author  onyettr
 * @file    tokenizer.h
 *******************************************************************************
 */
#ifndef __TOKENIZER__H__
#define __TOKENIZER__H__

/*
********************************************************************************
Includes
********************************************************************************
*/

/*
********************************************************************************
Private Types
********************************************************************************
*/

/**
 * @brief TOKEN values for the various elements
 * @enum  TokenValues_t
 * @addtogroup Tokenizer
 */
typedef enum {
    /*
     * Tokem main entities
     */              
    TOKEN_WORD,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPECIAL,
    TOKEN_NO_TOKEN,

    /*
     * QWERTY keyboard tokens
     */
    TOKEN_TILDE,
    TOKEN_OPEN_QUOTE,
    TOKEN_PLING,
    TOKEN_AT,
    TOKEN_HASH,
    TOKEN_DOLLAR,
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
     * Based on Dartmouth BASIC original dialect (Edition #1_
     * Reserved words tokens, 
     */
    TOKEN_LET,                   /* 1st edition */
    TOKEN_PRINT,
    TOKEN_END,
    TOKEN_READ,
    TOKEN_DATA,
    TOKEN_GOTO,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_FOR,
    TOKEN_TO, 
    TOKEN_NEXT,
    TOKEN_GOSUB,
    TOKEN_RETURN,
    TOKEN_DEF,
    TOKEN_DIM,
    TOKEN_REM,
    TOKEN_STOP,

    TOKEN_MAT,                    /* 2nd edition */

    TOKEN_SGN,                    /* 3rd edition */
    TOKEN_RESTORE,
    TOKEN_INPUT,

    TOKEN_RANDOMIZE,              /* 4th edition */
    TOKEN_ON,
    TOKEN_CHANGE,

    TOKEN_RESET,                  /* 5th edition */
    TOKEN_LOC,
    TOKEN_LOF,
    TOKEN_CHAIN,
    TOKEN_COMMON,
    TOKEN_TIM,
    TOKEN_LEN,
    TOKEN_STRDOLLAR,
    TOKEN_VAL,
    TOKEN_ASC,

    TOKEN_CALL,                   /* 6th edition */
    TOKEN_SUB,
    TOKEN_SUBEND,
    TOKEN_FILE,
    TOKEN_SEG,
    TOKEN_MID,
    
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
     * Dartmouth Time Sharing System (DTSS) Direct commands
     * Not part of the BASIC language, but used to run programmes
     */
    TOKEN_HELLO,                    /* Log onto DTSS                    */
    TOKEN_BYE,                      /* Log off  DTSS                    */
    TOKEN_BASIC,                    /* Start basic mode                 */
    TOKEN_NEW,                      /* Name and begin writing a program */
    TOKEN_OLD,                      /* Get previous program             */
    TOKEN_SAVE,                     /* Save current program             */
    TOKEN_REPLACE,                  /* Save using new filename          */
    TOKEN_RENAME,                   /* Change name                      */
    TOKEN_CAT,                      /* CATALOG display programs stored  */
    TOKEN_LIST,                     /* Display current program          */
    TOKEN_RUN,                      /* Execute current program          */
    TOKEN_CMD_STOP,                 /* Interrupt current program        */
    TOKEN_UNSAVE,                   /* clear current program            */
    TOKEN_SYSTEM,                   /* ??                               */
    TOKEN_GOODBYE,
    TOKEN_SCRATCH,                  /* erase current program            */
    TOKEN_FRI,                      /* Friden mode                      */
    TOKEN_NFR,                      /* Exit FRI mode                    */
    TOKEN_EXPLAIN,                  /* Help command                     */

    /*
     * Debug and extra commands: 
     */
    TOKEN_HELP,                     /* Help command                     */
    TOKEN_SYMTABLE_LIST,            /* SHow contents of Symbol Table    */
    
    TOKEN_EOF,
    TOKEN_OK, 
    TOKEN_ERROR
} TokenValues_t;

/**
 *  @brief Token_t enumeration for Tokens
 *  @addtogrpup Tokenizer
 */
typedef int32_t Token_t;

/**
 * @brief Literal Types string and integer
 * @enum LiteralType_t
 * @addtogroup Tokenizer
 */
typedef enum {
   LITERAL_STRING,
   LITERAL_FLOAT,
   LITERAL_INTEGER
} LiteralType_t;

/**
 * @brief Literal string and integer storage
 * @struct Literal_t 
 * @addtogroup Tokenizer
 */
typedef struct {
  LiteralType_t Type;
  union {
    int32_t IntegerValue;
    float   FloatValue;
    char    StringValue[MAX_SOURCE_LINE_LENGTH];
  } value;                   /*! union value */
} Literal_t;

/**
 * @brief  Command list definition
 * @struct TokenCommandList_t
 * @note   <String> <Token Value> <Callback_fn>
 *         if the string matches then the Token is returned and a callback can be invoked.
 *         Commands are DIRECT meaning system calls or they are language Keywords
 * @todo   Use the callback function for the direct commands
 * @addtogroup Tokenizer
 */
typedef struct {
  char *cmdstr;                      /*!> String containing Command                    */
  char *HelpString;                  /*!> parameters and help                          */
  Token_t TokenValue;                /*!> Token value for this command                 */
  //  int (*pDirectFunction)(int argc, char *argv[]);
  int32_t (*pDirectFunction)(void);  /*!> For future use to add callbacks for Keywords */
} TokenCommandList_t;

/*
********************************************************************************
Private variables (static)
********************************************************************************
*/

/*
********************************************************************************
Global variables
********************************************************************************
*/

/*
********************************************************************************
Exported Global variables
********************************************************************************
*/

/*
********************************************************************************
Prototypes of all functions contained in this file (in order of occurrence)
********************************************************************************
*/
int32_t Tokenize (char *FileName);
Token_t TokenGetNumber     (char **Bufferp, char *Tokenp, Token_t PreToken);
Token_t TokenGetWord       (char **Bufferp, char *Tokenp);
Token_t TokenGetString     (char **Bufferp, char *Tokenp);
Token_t TokenGetSpecial    (char **Bufferp, char *Tokenp);
Token_t TokenGetDirect     (char **Bufferp, char *Tokenp);
void    TokenPrint         (char *TokenString, Token_t Token);
Token_t TokenDirectKeyword (char *Bufferp);
Token_t TokenDirectCommand (char *Bufferp);
bool    IsTokenDirectCommand (char *Bufferp);
bool    IsTokenDirectKeyword (char *Bufferp);
int32_t TokenExecuteDirectCommand (Token_t CommandToken, char *Tokenp);
char    *TokenGetStringType(Token_t Token);

#endif  /* __TOKENIZER_H__ */
