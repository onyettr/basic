/**
 *****************************************************************************
 * @brief   basic interpreter - binary tree
 * @author  onyettr
 * @file    binarytree.c
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
SymbolTableNode_t *pSymbolTableRoot  = NULL;
#if defined(TEST_HARNESS)
bool Verbose = true;
#endif

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
 * @fn          *SymbolTableCreate(void)
 * @brief       Create a new SymbolTable
 * @return      NULL if error, otherwise new SymbolTable
 * @details     
 * @todo  
 * @note        none
 */
SymbolTableNode_t *SymbolTableCreate(void) {
  SymbolTableNode_t *pNewNode;

  pNewNode = malloc(sizeof(SymbolTableNode_t));
  pNewNode->pLeft        = NULL;
  pNewNode-> pRight      = NULL;
  pNewNode-> pNext       = NULL;
  pNewNode->NameString   = NULL;
  pNewNode->InfoString   = NULL;
  pNewNode->NestingLevel = 0;
  pNewNode->LabelIndex   = 0;
  
  if (Verbose) printf ("SymbolTableCreate %p\n", (void*)pNewNode);
  
  if (pNewNode == NULL) {
    //    Error("SymbolTableCreate - %s\n", ErrorToString(ERROR_SYMBOL_TABLE_FAILED_TO_ALLOCATE_NODE));
  }

  return pNewNode;                                         
}

/**
 * @fn          SymbolTableClean(void)
 * @brief       Clean up the allocated memory 
 * @return      none
 * @details     
 * @todo  
 * @note        none
 */
void SymbolTableClean(SymbolTableNode_t *pSymbolTable) {

  if (pSymbolTable) {
    SymbolTableClean(pSymbolTable->pLeft);
    SymbolTableClean(pSymbolTable->pRight);
    free(pSymbolTable->NameString);
    free(pSymbolTable->InfoString);
    free(pSymbolTable);
    if (Verbose) printf("SymbolTableClean: %p\n", (void*)pSymbolTable);
  }
}

/**
 * @fn          *SymbolTableSearch (char *Name, SymbolTableNode_t *pNode) {
 * @brief       Searches the Symbol Table for Name
 * @param[in]   *Name    - Name to search for
 * @param[in]   *pNode   - Which tree to search in
 * @return      NULL if not found, SymbolTableNodePtr if found
 * @details     
 * @todo        return bool?
 * @note        none
 */
SymbolTableNode_t *SymbolTableSearch (char *Name, SymbolTableNode_t *pNode) {

  if (Verbose) printf("SymbolTableSearch %s, %p\n", Name, (void*)pNode);
  
  while (pNode != NULL) {
    int Compare = 0;     

    if (Verbose) printf("SymbolTableSearch [%p] %s, %s, cmp %d\n", (void*)pNode, Name, pNode->NameString, Compare);
    //if (Verbose) printf("SymbolTableSearch [%p] %s\n", (void*)pNode, Name);    
    
    Compare = strcmp(Name,pNode->NameString);

    if (Verbose) printf("SymbolTableSearch [%p] %s, %s, cmp %d\n", (void*)pNode, Name, pNode->NameString, Compare);
    // if (Verbose) printf("SymbolTableSearch [%p] %s\n", (void*)pNode, Name);
    
    if (Compare == 0) {
      return pNode;                                        /* Search matched      */
    }

    pNode = (Compare < 0) ? pNode->pLeft : pNode->pRight;  /* Continue the search */
  }

  return NULL;                                             /* No match found      */
}

/**
 * @fn          SymbolTableNodePtr_t *SymbolTableAddName (char *Name, SymbolTableNode_t **pNode) {
 * @brief       Add a new Node to the symbol table
 * @param[in]   *Name    - Name to search for
 * @param[in]   **pNode  - Symbol table to add to 
 * @return      
 * @details     
 * @todo  
 * @note        none
 */
SymbolTableNode_t *SymbolTableAddName (char *Name, SymbolTableNode_t **pNode) {
  SymbolTableNode_t *pNewNode = NULL;
  SymbolTableNode_t *pNodeTest = NULL;  

  if (Verbose) printf("SymbolTableAddName: pNode %p, *pNode %p\n", (void*)pNode, (void*)*pNode);
  
  /*
   * Create a new leaf node and fill in the details
   * TODO Needs a bit more checking here!
   */
  pNewNode = malloc(sizeof(SymbolTableNode_t));
  pNewNode->NameString = malloc(strlen(Name)+1);
  strcpy(pNewNode->NameString, Name);

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
  while ((pNodeTest = *pNode) != NULL) {
    int Compare = 0;    
    //    printf("1st cmp pNode %p pNodeTest %p\n", (void*)pNode, (void*)pNodeTest);    

    Compare = strcmp(Name,pNodeTest->NameString);
    pNode = (Compare < 0) ? &(pNodeTest->pLeft) : &(pNodeTest->pRight);
    //    printf("2nd cmp pNode %p pNodeTest %p\n", (void*)pNode, (void*)pNodeTest);
  }

  *pNode = pNewNode;

  printf("Return %p = %p\n", (void*)*pNode, (void*)pNewNode);
  
  return pNewNode;                                
}

/**
 * @fn          void SymbolTableShow (SymbolTableNode_t *pNode) 
 * @brief       List the contents of the Symbol Table
 * @param[in]   *pNode  - Symbol table to list
 * @return      
 * @details     Using recursion, been a while..
 * @todo  
 * @note        none
 */
void SymbolTableShow (SymbolTableNode_t *pNode) {
  static bool BannerPrint = true;

  if (BannerPrint) {
    if (pNode== NULL) {
      printf("*** Symbol table is empty ***\n");

      return;
    }
    
    printf("\t\t    Left\t    Right\t\tNext\t\tName\n");
    
    BannerPrint = false;
  }

  if (pNode) {
    SymbolTableShow(pNode->pLeft);    

    printf("[%8p]\t%8p\t%8p\t%8p\t%20s\n",
           (void*)pNode,
           (void*)pNode->pLeft,
           (void*)pNode->pRight,
           (void*)pNode->pNext,
           (char*)pNode->NameString);           

    SymbolTableShow(pNode->pRight);    
  }
}

/**
 * @fn          int32_t TT_SymbolTableShow (void)
 * @brief       Callharness from Direct Command Table
 * @param[in]   
 * @return      
 * @details     
 * @todo  
 * @note        none
 */
#if 0
int32_t TT_SymbolTableShow (void) {
  SymbolTableShow(pSymbolTableRoot);

  return SUCCESS;
}
#endif

static void Padding ( char ch, int n) {
  int i;

  for (i=0; i < n; i++) {
    printf("%c", ch);
  }
}

void SymbolTableShowNew (SymbolTableNode_t *pNode, int Level) {

  if (pNode == NULL) {
    Padding(' ', Level);
    printf("~");
  } else {
    SymbolTableShowNew(pNode->pRight,Level+1);
    Padding(' ', Level);

    printf("%s\n", pNode->NameString);
    
    SymbolTableShowNew(pNode->pLeft, Level+1);        
  }          
}

/*
 * Module test harness for the binarytree operations
 */
#if defined(TEST_HARNESS)
int main (void) {
  SymbolTableNode_t *pSymbolRoot  = NULL;
  SymbolTableNode_t *pNewNode     = NULL;
  char NodeNames[][16] = {
                       "bNode1",
                       "cNode2",
                       "aNode3",
                       "aaNod4",
                       "zNode5",
                       "wNode6",
                       "zzNod7",
                       "aaaNo8",
                       "aaaaa9",
                       "bbzz10",
                       "longerNode",
		       "varx",
		       "x",
		       "pi"
  };
  int i = 0;
  int Dimension = sizeof(NodeNames)/sizeof(NodeNames[0]);
  printf("%s Test Harness %d\n", __FILE__, Dimension);

  for (i=0; i < 13; i++) {
    pNewNode = SymbolTableSearch  (NodeNames[i], pSymbolRoot);
    if (pNewNode == NULL) {
      pNewNode = SymbolTableAddName(NodeNames[i], &pSymbolRoot);
    }
  }
  SymbolTableShow (pSymbolRoot);

  SymbolTableShowNew (pSymbolRoot, 0);  

  SymbolTableClean(pSymbolRoot);

  UNUSED(pNewNode);
  UNUSED(pSymbolRoot);
}
#endif


