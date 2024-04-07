/**
 *******************************************************************************
 * @brief    Basic interpreter
 * @author   onyettr
 * @file     symboltable.h
 * @defgroup Symboltable Symboltable
 * @ingroup  Symboltable
 *           Utilities to drive the symbol table
 *******************************************************************************
 */
#ifndef __SYMBOLTABLE__H__
#define __SYMBOLTABLE__H__

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
 *  @brief Value_t enumeration for values
 *  @addtogroup Utilities
 */
typedef union {
  int32_t IntegerValue;        /*!> Integer value        */
  float   RealValue;           /*!> Floating point value */
  uint8_t CharacterValue;      /*!> Character value      */
  uint8_t *String;             /*!> String value         */
} Value_t;

/**
 *  @brief Definition key
 *  @enum DefinitionKey_t
 *  @addtogroup Utilities
 */
typedef enum {
  KEY_UNDEFINED,               /*!> Variable is undefined *//**< KEY_UNDEFINED */
  KEY_CONSTANT,                /*!> variable is constant  *//**< KEY_CONSTANT */
  KEY_VARIABLE                 /*!> variable is variable  *//**< KEY_VARIABLE */
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
  DefinitionStruct_t      Defun;  
  char *NameString;
  char *InfoString;
  int32_t NestingLevel;
  int32_t LabelIndex;
} SymbolTableNode_t, *SymbolTableNodePtr_t;

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
SymbolTableNode_t *symbol_table_search (char *Name, SymbolTableNode_t *pNode);
SymbolTableNode_t *symbol_table_add_name (char *Name, SymbolTableNode_t **pNode);
SymbolTableNode_t *symbol_table_add_node (char *Name, SymbolTableNode_t **pNode);
SymbolTableNode_t *symbol_table_create (SymbolTableNode_t **pRootNode);
SymbolTableNode_t *symbol_table_create_node (char *pNameString);
void symbol_table_clean (SymbolTableNode_t *pSymbolTable);
void symbol_table_show  (SymbolTableNode_t *pNode);

int32_t TT_SymbolTableShow (void);
#endif  /* __SYMBOLTABLE__H__ */








