/**
 *****************************************************************************
 * @brief   basic interpreter - symbol table
 * @author  onyettr
 * @file    symboltable.c
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
SymbolTableNodePtr_t SymbolTableRoot;

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
Prototypes of all functions contained in this file (in order of occurance)
******************************************************************************
*/

/**
 * @fn          SymbolTableNodePtr SearchSymbolTable (char *Name, SymbolTableNode_t *pNode) {
 * @brief       Searches the Symbol Table for Name
 * @param[in]   *Name    - Name to search for
 * @param[in]   *pNode   - Which tree to search in
 * @return      NULL if not found, SymbolTableNodePtr if found
 * @details     
 * @todo  
 * @note        none
 */
SymbolTableNode_t *SearchSymbolTable (char *Name, SymbolTableNode_t *pNode) {
  int Compare = 0;

  printf("Search %s, %p\n", Name, (void*)pNode);
  while (pNode != NULL) {
    Compare = strcmp(Name,pNode->NameString);
    if (Compare == 0) {
      return pNode;                                        /* Search matched      */
    }

    pNode = (Compare < 0) ? pNode->pLeft : pNode->pRight;  /* Continue the search */
  }

  return NULL;                                             /* No match found      */
}

/**
 * @fn          AddNameToSymbolTableNodePtr_t AddSymbolTable (char *Name, SymbolTableNode_t **pNode) {
 * @brief       Add a new Node to the symbol table
 * @param[in]   *Name    - Name to search for
 * @param[in]   **pNode  - Symbol table to add to 
 * @return      
 * @details     
 * @todo  
 * @note        none
 */
SymbolTableNodePtr_t AddNameToSymbolTable (char *Name, SymbolTableNode_t **pNode) {
  SymbolTableNode_t *pNewNode;
  SymbolTableNode_t *pNodeTest;  
  int Compare = 0;

  printf("pNode %p, *pNode %p\n", (void*)pNode, (void*)*pNode);
  
  /*
   * TODO Needs a bit more checking here!
   */
  pNewNode = malloc(sizeof(SymbolTableNode_t));
  pNewNode->NameString = malloc(strlen(Name)+1);
  strcpy(pNewNode->NameString, Name);

  printf("alloc %p\n", (void*)pNewNode);
  
  pNewNode->pLeft = NULL;
  pNewNode->pRight= NULL;
  pNewNode->pNext = NULL;
  pNewNode->InfoString = NULL;
  pNewNode->Defun.Key = KEY_UNDEFINED;
  pNewNode->NestingLevel = 0;
  pNewNode->LabelIndex   = 0;

  /*
   * Look for insertion point
   */
  while ( (pNodeTest = *pNode) != NULL) {
    printf("1st cmp pNode %p pNodeTest %p\n", (void*)pNode, (void*)pNodeTest);    
    Compare = strcmp(Name,pNodeTest->NameString);
    pNode = (Compare < 0) ? &(pNodeTest->pLeft) : &(pNodeTest->pRight);
    printf("2nd cmp pNode %p pNodeTest %p\n", (void*)pNode, (void*)pNodeTest);
  }

  *pNode = pNewNode;
  printf("Return %p = %p\n", (void*)*pNode, (void*)pNewNode);
  
  return pNewNode;                                
}

/**
 * @fn          void ShowSymbolTable (SymbolTableNode_t *pNode) 
 * @brief       List the contents of the Symbol Table
 * @param[in]   *pNode  - Symbol table to list
 * @return      
 * @details     
 * @todo  
 * @note        none
 */
void ShowSymbolTable (SymbolTableNode_t *pNode) {
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



