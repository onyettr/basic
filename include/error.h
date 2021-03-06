/**
 *****************************************************************************
 * @brief   basic interpreter
 * @author  onyettr
 * @file    error.h
 ***************************************************************************** 
 */
#ifndef __ERROR__H__
#define __ERROR__H__

/*
******************************************************************************
Includes
******************************************************************************
*/

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

/**
 * @brief Error codes
 * @enum ErrorCode_t
 */
typedef enum {
  SUCCESS = 1,
  ERROR_FILE_OPEN_FILENAME,
  ERROR_FILE_OPEN_FAILURE,
  ERROR_FILE_NO_FILENAME,
  ERROR_NUMBER_TOO_LARGE,                         /* 9 digits only allowed */
  ERROR_SYMBOL_TABLE_FAILED_TO_ALLOCATE_NODE,
  ERROR_SYMBOL_TABLE_FAILED_TO_ALLOCATE_STRING,
  ERROR_UNKNOWN
} ErrorCode_t;

typedef struct {
  char        *ErrorString;
  ErrorCode_t ErrorCode;
} ErrorCodeList_t;

void Error (char *format, ...);
char *ErrorToString (ErrorCode_t ErrorCode);

#endif  /* _ERROR_H__ */
