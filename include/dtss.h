/**
 *****************************************************************************
 * @brief   basic interpreter
 * @author  onyettr
 * @file    dtss.h
 ***************************************************************************** 
 */
#ifndef __DTSS__H__
#define __DTSS__H__

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
Prototypes of all functions contained in this file (in order of occurance)
******************************************************************************
*/
int32_t  DTSSCommandHello  ( void );
int32_t  DTSSCommandNew    ( void );
int32_t  DTSSCommandOld    ( void );
int32_t  DTSSCommandSave   ( void );
int32_t  DTSSCommandReplace( void );
int32_t  DTSSCommandRename ( void );
int32_t  DTSSCommandCat    ( void );
int32_t  DTSSCommandList   ( void );
int32_t  DTSSCommandRun    ( void );
int32_t  DTSSCommandStop   ( void );
int32_t  DTSSCommandUnsave ( void );
int32_t  DTSSCommandSystem ( void );
int32_t  DTSSCommandScratch( void );
int32_t  DTSSCommandExplain( void );

#endif  /* _ERROR_H__ */
