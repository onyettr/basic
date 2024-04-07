/**
 *****************************************************************************
 * @brief   basic interpreter - symbol table
 * @author  onyettr
 * @file    symboltable.c
 * @ingroup Symboltable
 * @ingroup Utilities
 ***************************************************************************** 
 */

/*
******************************************************************************
Includes
******************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
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
SymbolTableNodePtr_t SymbolTableRoot; /*!< Start of symbol table             */

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
 * @fn          SymbolTableNodePtr symbol_table_search (char *Name, SymbolTableNode_t *pNode) {
 * @brief       Searches the Symbol Table for Name
 * @param[in]   Name    - Name to search for
 * @param[in]   pNode   - Which tree to search in
 * @return      NULL if not found, SymbolTableNodePtr if found
 * @details     
 * @todo  
 * @note
 */
SymbolTableNode_t *symbol_table_search (char *Name, SymbolTableNode_t *pNode) {

  printf("Search %s, %p\n", Name, (void*)pNode);
  
  while (pNode != NULL) {
    int Compare = 0;
    
    Compare = strcmp(Name,pNode->NameString);
    if (Compare == 0) {
      return pNode;                                   /* Search matched      */
    }

    pNode = (Compare < 0) ? pNode->pLeft : pNode->pRight;  /* Continue the search */
  }

  return NULL;                                        /* No match found      */
}

/**
 * @fn          SymbolTableNode_t *symbol_table_create (SymbolTableNode_t **pRootNode)
 * @brief       create a new symbol table (root node)
 * @param[out]  *pRootNode - name of new symbol table
 * @return      NULL if not found, SymbolTableNodePtr if found
 * @details     
 * @todo        requires implementing
 * @note        none
 */
SymbolTableNode_t *symbol_table_create(SymbolTableNode_t **pRootNode) {
  if (Verbose) {
	  printf("symbol_table_create %p\n", (void*)pRootNode);
  }

  return NULL;
}

/**
 * @fn          SymbolTableNode_t *symbol_table_create_node (char *pNameString)
 * @brief       create a new symbol table node
 * @param[in]   *pNameString - name to store in symbol table
 * @return      NULL if failure, SymbolTableNodePtr if not
 * @details     
 * @todo        
 * @note        none
 */
SymbolTableNode_t *symbol_table_create_node (char *pNameString) {
  SymbolTableNode_t *pNewNode = NULL;
  
  if (Verbose) {
	  printf("CreateSymbolTableNode STARTS\n");
  }

  pNewNode = malloc(sizeof(SymbolTableNode_t));
  if (pNewNode == NULL) {
    Error("CreateSymbolTableNode - %s",
    	ErrorToString(ERROR_SYMBOL_TABLE_FAILED_TO_ALLOCATE_NODE));

    return NULL;
  }
  
  pNewNode->NameString = malloc(strlen(pNameString)+1);
  if (pNewNode->NameString == NULL) {
    Error("CreateSymbolTableNode - %s",
    		ErrorToString(ERROR_SYMBOL_TABLE_FAILED_TO_ALLOCATE_STRING));

    return NULL;
  }
  strcpy(pNewNode->NameString, pNameString);
  
  return pNewNode;
}

/**
 * @fn          SymbolTableNodePtr_t symbol_table_add_node
 *                               (char *Name, SymbolTableNode_t **pNode)
 * @brief       Add a new Node to the symbol table
 * @param[in]   *Name    - Name to search for
 * @param[out]  **pNode  - Symbol table to add to
 * @return      
 * @details     
 * @todo  
 * @note        none
 */
SymbolTableNodePtr_t symbol_table_add_node (char *Name,
		                                    SymbolTableNode_t **pNode)
{
  SymbolTableNode_t *pNewNode;
  SymbolTableNode_t *pNodeCurrent;
  SymbolTableNode_t *pNodeParent;    
  bool Constructing = true;

  printf("pNode %p, *pNode %p\n", (void*)pNode, (void*)*pNode);
  
  /*
   * Create a new node for this entry
   */
  pNewNode = symbol_table_create_node(Name);

  printf("alloc %p\n", (void*)pNewNode);
  
  pNewNode->pLeft = NULL;
  pNewNode->pRight= NULL;
  pNewNode->pNext = NULL;
  pNewNode->InfoString = NULL;
  pNewNode->Defun.Key = KEY_UNDEFINED;
  pNewNode->NestingLevel = 0;
  pNewNode->LabelIndex   = 0;

  if (*pNode == NULL) {  /* There is no Root */
    printf("Root node created %p\n", pNode);
    *pNode = pNewNode;

    return *pNode;
  }
  
  pNodeCurrent = *pNode;
  pNodeParent  = NULL;

  while (Constructing) {
    int Compare = 0;

    pNodeParent = pNodeCurrent;
    Compare = strcmp(Name,pNodeParent->NameString);
    printf("cmp %d Parent %p Current %p\n", Compare, (void*)pNodeParent,
           (void*)pNodeCurrent);
    if (Compare < 0) {
      pNodeCurrent = pNodeCurrent->pLeft;

      if (pNodeCurrent == NULL) {
        pNodeParent->pLeft = pNewNode;

        Constructing = false;
      }
    } else {   /* Insert right */
      pNodeCurrent = pNodeCurrent->pRight;

      if (pNodeCurrent == NULL) {
        pNodeParent->pRight = pNewNode;

        Constructing = false;
      }
    }
  }

#if 0
  /*
   * Look for insertion point
   */
  while ( (pNodeTest = *pNode) != NULL) {
    printf("1st cmp pNode %p pNodeTest %p\n", (void*)pNode, (void*)pNodeTest);    
    Compare = strcmp(Name,pNodeTest->NameString);
    pNode = (Compare < 0) ? &(pNodeTest->pLeft) : &(pNodeTest->pRight);
    printf("2nd cmp pNode %p pNodeTest %p\n", (void*)pNode, (void*)pNodeTest);
  }
#endif
  
  *pNode = pNewNode;
  printf("Return %p = %p\n", (void*)*pNode, (void*)pNewNode);

  return pNewNode;
}

/**
 * @brief
 *
 * @param pNode
 */
void symbol_table_clean(SymbolTableNode_t *pNode) {
 UNUSED(pNode);
}


/**
 * @fn          void symbol_table_show (SymbolTableNode_t *pNode)
 * @brief       List the contents of the Symbol Table
 * @param[in]   *pNode  - Symbol table to list
 * @return      
 * @details     
 * @todo  
 * @note        none
 */
void symbol_table_show (SymbolTableNode_t *pNode) {
  SymbolTableNode_t *pLeftNode  = pNode;
  //  SymbolTableNode_t *pRightNode = pNode;  

  printf("\t\tLeft\tRight\tNext\tName\n");
  while (pLeftNode != NULL) {
    printf("[%8p]\t%8p\t%8p\t%8p\t%20s\n",
           (void*)pNode,
           (void*)pNode->pLeft,
           (void*)pNode->pRight,
           (void*)pNode->pNext,
           (char*)pNode->NameString);           
    pLeftNode = pNode->pNext;
  }
#if 0    
  while (pRightNode != NULL) {
    printf("[%8p]\t%8p\t%8p\t%8p\t%20s\n",
           (void*)pNode,
           (void*)pNode->pLeft,
           (void*)pNode->pRight,
           (void*)pNode->pNext,
           (char*)pNode->NameString);            
   pRightNode = pNode->pRight;
  }
#endif  
}
