/**
 *****************************************************************************
 * @brief   basic interpreter - Error handling
 * @author  onyettr
 * @file    error.c
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
static ErrorCodeList_t ErrorRecord[] = {
   { "OK"                ,  SUCCESS                  }, 
   { "File open error "  ,  ERROR_FILE_OPEN_FILENAME },
   { "File no filename"  ,  ERROR_FILE_NO_FILENAME   },
   { "Number is too big" ,  ERROR_NUMBER_TOO_LARGE   },
   { NULL                ,  ERROR_UNKNOWN            }
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
 * @brief     Convert ErrorCode number into an error string
 * @fn        char *ErrorToString (ErrorCode_t ErrorCode)
 * @param[in] ErrorCode
 * @return    char *
 * @details   
 * @note
 * @todo     
 */
char *ErrorToString (ErrorCode_t ErrorCode) {
  ErrorCodeList_t *pRow;

  pRow = (ErrorCodeList_t *)&ErrorRecord[0];  
  while (pRow->ErrorString != NULL) {
    if (pRow->ErrorCode == ErrorCode) {
       return pRow->ErrorString;
    }
    pRow++;
  }    
 
  return pRow->ErrorString;
}

/**
 * @fn          void Error ( char *format, ... ) {
 * @brief       standard error output formatter
 * @param[in]   *format, ... number of formatted argumantes
 * @return      none
 * @note        outputs error string
 */
void Error ( char *format, ... ) {
    va_list ap;
    char Buffer[256];

    va_start (ap, format);
    (void)vsprintf (Buffer, format, ap);
    va_end (ap);

    printf ("Error-%s-%s\n", PROGRAM_NAME, Buffer );

    exit (0);
}



