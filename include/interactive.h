/**
 *****************************************************************************
 * @brief    Basic interpreter
 * @author   onyettr
 * @file     interactive.h
 * @defgroup Interactive Interactive
 * @ingroup  Interactive
 *           Ability to tokenize and parse via the command line
 ***************************************************************************** 
 */
#ifndef __INTERACTIVE__H__
#define __INTERACTIVE__H__

/*
******************************************************************************
Includes
******************************************************************************
*/
#include "tokenizer.h"

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
int32_t CommandLineMode (void);
int32_t TT_InteractiveHelp (void);

#endif  /* __INTERACTIVE_H__ */
