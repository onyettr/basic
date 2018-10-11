/**
 *****************************************************************************
 * @brief   C Programming Examples - basic interpreter
 * @author  onyettr
 * @file    main.c
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
    "basic: interpretor                        \n"
    "Usage: basic {option} {filename}        \n\n"
    "Options:                                \n\n"
    "l   list programme                        \n"
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

FILE *SourceFile;

char SourceFileName   [MAX_FILENAME_LENGTH];
char SourceLineBuffer [MAX_SOURCE_LINE_LENGTH];

uint32_t LineNumber = 0;

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
 * @fn          void Error ( char *format, ... ) {
 * @brief       standard error output formatter
 * @param[in]   char*  format, ... number of formatted argumantes
 * @return      none
 * @note        outputs error string
 */
void Error ( char *format, ... ) {
    va_list ap;
    char Buffer[256];

    va_start ( ap, format );
    (void)vsprintf ( Buffer, format, ap);
    va_end ( ap );

    printf ("Error-%s-%s\n", PROGRAM_NAME, Buffer );

    exit (0);
}

/**
 * @fn          static void ParseCommandLine ( char *argv[], int argc, char *InputFileName)
 * @brief       Parse the optins from argv
 * @param[in]   char** argv  The arguments.
 * @param[in]   int    argc  The numer of arguments.
 * @param[out]  char*  InputFileName
 * @return      none
 * @note        breaks down the command line
 */
static void ParseCommandLine ( char *argv[], int argc, char *InputFileName ) {
  int i = 1;
  char *ch;

  /*
   * Test for any args at all
   */
  if ( argc == 1 ) {
      fprintf (stdout, "%s\n", USAGE_STRING);
      exit (0);
  }
  
  /*
   * Go round and pull in all the commands on the CLI.
   */
  while (i < argc) {
     ch = argv[i++];    /* Get the whole argument    */

     switch (*ch++) {
        case '/':
        case '-': {
	   switch (*ch++) {
	      case 'v':
	      case 'V': Verbose = true; break;
	      case 'l':
	      case 'L': ListMode = true; break;
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

int main ( int argc, char *argv[]) {
  char FileName[80];   

  /*
   * Sign on banner..
   */
  printf ("%s: Version %s\n", PROGRAM_NAME, VERSION);

  ParseCommandLine (argv, argc, FileName);

  SourceLister (FileName);
  
  return 0;
}
