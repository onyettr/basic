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
 * @ingroup  Misc
 *****************************************************************************
 */

/*
******************************************************************************
Includes
******************************************************************************
*/
#include "basic.h"
#include "debug.h"

#ifndef GIT_REPO
#define GIT_REPO "unknown"
#endif

#ifndef GIT_COMMIT
#define GIT_COMMIT "unknown"
#endif

#ifndef GIT_BRANCH
#define GIT_BRANCH "unknown"
#endif

#ifndef BUILD_DATE
#define BUILD_DATE "unknown"
#endif

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

/**
 *
 */
static void print_banner(void) {
	printf ("%s: Version %s %s %s\n",
			PROGRAM_NAME, VERSION,
	        GIT_BRANCH, BUILD_DATE);
}

int main ( int argc, char *argv[]) {
  char FileName[80];

  FileName[0] = '\0';

  /*
   * Sign on banner..
   */
  print_banner();

  parse_command_line (argv, argc, FileName);
  DEBUG_init(true, verbose_level);
  DEBUG_set_categories(DEBUG_CAT_TOKENIZER | DEBUG_CAT_SYMBOLS);

  if (ListMode) {
    ListerSource (FileName);          /* List the source code to the console */
    //  } else if (FileName[0] != '\0') {
  } else if (ScanMode) {
 //   Tokenize(FileName);
	Tokenize_v2(FileName);

  } else {
    CommandLineMode();
  }

  return 0;
}
