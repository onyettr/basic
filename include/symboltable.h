/**
 *****************************************************************************
 * @brief    Basic interpreter
 * @author   onyettr
 * @file     symboltable.h
 * @defgroup symboltable
 *           Utilities to drive the symbol table
 ***************************************************************************** 
 */
#ifndef __SYMBOLTABLE__H__
#define __SYMBOLTABLE__H__

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
 *  @brief Value_t enumeration for vakues
 *  @addtogrpup Utilities
 */
typedef union {
  int32_t IntegerValue;        /*!> Integer value        */
  float   RealValue;           /*!> Floating point value */
  uint8_t CharacterValue;      /*!> Character value      */
  uint8_t *String;             /*!> String valie         */
} Value_t;

typedef enum {
  KEY_UNDEFINED,               /*!> Variable is undefined */
  KEY_CONSTANT,                /*!> variable is constant  */
  KEY_VARIABLE                 /*!> variable is variable  */
} DefinitionKey_t;

typedef struct {
  DefinitionKey_t Key;
  union {
    struct {
      Value_t value;
    } Constant;

    struct {
      int32_t ParameterCount;
      int32_t TotalLocalSize;
      int32_t TotalParameterSize;
    } Routine;
  } Information;
} DefinitionStruct_t;

typedef struct SymbolTableNode {
  struct SymbolTableNode *pLeft;
  struct SymbolTableNode *pRight;  
  struct SymbolTableNode *pNext;
  char *NameString;
  char *InfoString;
  DefinitionStruct_t      Defun;
  int32_t NestingLevel;
  int32_t LabelIndex;
} SymbolTableNode_t, *SymbolTableNodePtr_t;

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
SymbolTableNode_t    *SearchSymbolTable (char *Name, SymbolTableNode_t *pNode);
SymbolTableNodePtr_t AddNameToSymbolTable(char *Name, SymbolTableNode_t **pNode);
SymbolTableNodePtr_t ShowSymbolTable   (SymbolTableNode_t *pNode);
  
#endif  /* __SYMBOLTABLE__H__ */
