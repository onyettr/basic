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
    "basic: interpreter                               \n"
    "Usage: basic {option} {filename}               \n\n"
    "Options:                                       \n\n"
    " -l         list programme                       \n"
    " -s         scanner output                       \n"
    " -v[level]  Verbose mode (level 0-9, default: 1) \n"
    "               DEBUG_LEVEL_OFF    0              \n"
    "               DEBUG_LEVEL_ERROR  1              \n"
    "               DEBUG_LEVEL_WARN   2              \n"
    "               DEBUG_LEVEL_INFO   3              \n"
    "               DEBUG_LEVEL_TRACE  4              \n"
    "               DEBUG_LEVEL_DETAIL 5              \n"
	" -h         display help                         \n"
	" -z         secret mode                          \n"
	"\n"
	"Examples: \n"
	"  basic -v program.bas       (verbose level 1)   \n"
	"  basic -v3 program.bas      (verbose level 3)   \n"
	"  basic -s -v 2 program.bas  (scanner + verbose level 2) \n"
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
uint32_t verbose_level = 1;

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
 * @fn          void parse_command_line ( char *argv[], int argc, char *InputFileName)
 * @brief       Parse the optins from argv
 * @param[in]   *argv[] -  The arguments.
 * @param[in]   argc    - The number of arguments.
 * @param[out]  *InputFileName - return any filename found
 * @return      none
 * @note        breaks down the command line
 */
void parse_command_line (char *argv[], int argc, char *InputFileName) {
  int i = 1;
  bool filename_set = false;

  while (i < argc) {
     char *ch = argv[i++];

     if (*ch == '/' || *ch == '-') {
        ch++; // Move past the prefix
        switch (*ch) {
        	case 's':
            case 'S': ScanMode = true; break;
            case 'v':
            case 'V':
            	Verbose = true;
            	// Check if there's a level number following
                if (*ch && *(ch + 1) >= '0' && *(ch + 1) <= '9') {
                	verbose_level = *(ch + 1) - '0';
                } else if (i < argc && argv[i][0] >= '0' && argv[i][0] <= '9') {
                   // Level is the next argument
                	verbose_level = atoi(argv[i++]);
                } else {
                	verbose_level = 1; // Default level
                }
            	break;
            case 'l':
            case 'L': ListMode = true; break;
            case 'h':
            case 'H':
              fprintf(stdout, "%s\n", USAGE_STRING);
              exit(0);
            case 'Z':
            case 'z': Secret = true; break;
            default:
               Error("Invalid command line arg %s", argv[i-1]);
               break;
        }
     } else {
        // Assume it's the filename
        if (filename_set) {
           Error("Multiple filenames specified");
        }
        strncpy(InputFileName, argv[i-1], MAX_FILENAME_LEN - 1);
        InputFileName[MAX_FILENAME_LEN - 1] = '\0';
        filename_set = true;
     }
  }
 }

