/**
 *****************************************************************************
 * @brief    basic interpreter
 * @author   onyettr
 * @file     main.c
 * 
 * @mainpage Basic Interpreter 
 *           Project to implement a BASIC language interpreter using ANSI C
 *           BASIC is based on the Dartmouth Basic Edition #1 (1964)
 *           Supports interactive as well as batch mode via a file
 *  
 * @subpage  InteractiveMode "Interactive Mode"
 *
 * @subpage  BatchMode       "Batch Mode"
 *
 * @page     BatchMode
 * @ref      Tokenizer "Tokenizer"
 *
 * @page     InteractiveMode
 * @ref      Interactive "Interactive"
 *
 * @defgroup Tokenizer
 *
 * @defgroup Interactive
 ***************************************************************************** 
 */

/*
******************************************************************************
Includes
******************************************************************************
*/
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

int main ( int argc, char *argv[]) {
  char FileName[80];   

  FileName[0] = '\0';

  /*
   * Sign on banner..
   */
  printf ("%s: Version %s\n", PROGRAM_NAME, VERSION);

  ParseCommandLine (argv, argc, FileName);

  if (ListMode) {
    ListerSource (FileName);          /* List the source code to the console */
    //  } else if (FileName[0] != '\0') {
  } else if (ScanMode) {    
    Tokenize(FileName);
  } else {
    CommandLineMode();
  }
  
  return 0;
}
