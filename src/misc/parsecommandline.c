/**
 *****************************************************************************
 * @brief   basic interpreter
 * @author  onyettr
 * @file    parsecommandline.c
 * @ingroup Misc
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
static char USAGE_STRING[] = {
    "basic: interpreter                        \n"
    "Usage: basic {option} {filename}        \n\n"
    "Options:                                \n\n"
    "l   list programme                        \n"
    "s   scanner output                        \n"
    "v   Verbose Mode                          \n"
};

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
bool Verbose  = false;
bool Secret   = false;
bool ListMode = false;
bool ScanMode = false;

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
 * @fn          void ParseCommandLine ( char *argv[], int argc, char *InputFileName)
 * @brief       Parse the optins from argv
 * @param[in]   *argv[] -  The arguments.
 * @param[in]   argc    - The number of arguments.
 * @param[out]  *InputFileName - return any filename found
 * @return      none
 * @note        breaks down the command line
 */
void ParseCommandLine (char *argv[], int argc, char *InputFileName) {
  int i = 1;

  /*
   * Test for any args at all
   */
  //  if ( argc == 1 ) {
  //      fprintf (stdout, "%s\n", USAGE_STRING);
  //      exit (0);
  //  }
  
  /*
   * Go round and pull in all the commands on the CLI.
   */
  while (i < argc) {
     char *ch = argv[i++];    /* Get the whole argument    */

     switch (*ch++) {
        case '/':
        case '-': {
	   switch (*ch++) {
              case 's':
              case 'S': ScanMode = true; break;
	      case 'v':
	      case 'V': Verbose = true; break;
	      case 'l':
	      case 'L': ListMode = true; break;
	      case 'h':
              case 'H': fprintf (stdout, "%s\n", USAGE_STRING); exit(0); break;
	      case 'Z':
	      case 'z': Secret = true; break;
	      default: {  		 /* Not a valid CLI option, exit logging error */
		Error ( "Invalid command line arg %s", argv[i-1] );
            break;
          }
        } /* end switch */
        break;
      } /* end case */
      default: {
        /* We'll assume this is the filename */
        char TempHold[80];

        strcpy (TempHold, argv[i-1]);
        strcpy (InputFileName, TempHold);
        break;
      }
    }
  }
}

