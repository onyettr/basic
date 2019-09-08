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
 * @enum TokenValues_t
 */
typedef enum {
    TOKEN_WORD,
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPECIAL,
    TOKEN_ERROR
} TokenValues_t;

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
    char    String[MAX_SOURCE_LINE_LENGTH];
  } value;                   /*! union value */
} Literal_t;
  
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

#endif  /* __TOKENIZER_H__ */
