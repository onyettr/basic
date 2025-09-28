/**
 *******************************************************************************
 * @brief    Basic interpreter
 * @author   onyettr
 * @file     debug.h
 * @defgroup  debug debug
 * @ingroup  utilities
 *           Standard DEBUG functions
 *******************************************************************************
 */
#ifndef DEBUG__H
#define DEBUG__H

/*
********************************************************************************
Includes
********************************************************************************
*/

/*
********************************************************************************
Private Types
*******************************************************************************
*/
/* Debug levels */
#define DEBUG_LEVEL_OFF    0
#define DEBUG_LEVEL_ERROR  1
#define DEBUG_LEVEL_WARN   2
#define DEBUG_LEVEL_INFO   3
#define DEBUG_LEVEL_TRACE  4
#define DEBUG_LEVEL_DETAIL 5

/* Debug categories - can be combined with bitwise OR */
#define DEBUG_CAT_GENERAL    0x01
#define DEBUG_CAT_TOKENIZER  0x02
#define DEBUG_CAT_PARSER     0x04
#define DEBUG_CAT_SYMBOLS    0x08
#define DEBUG_CAT_IO         0x10

static uint32_t DebugCategories = DEBUG_CAT_GENERAL;

/*
******************************************************************************
Debug Function Prototypes
******************************************************************************
*/
void DEBUG_init(bool enable, int32_t level);
void DEBUG_set_level(int32_t level);
void DEBUG_set_categories(uint32_t categories);
void DEBUG_enable_category(uint32_t category);
void DEBUG_disable_category(uint32_t category);
void DEBUG_print(int32_t level, uint32_t category, const char *function,
                 int32_t line, const char *format, ...);
void DEBUG_dump_buffer(const char *label, const char *buffer, size_t length);
void DEBUG_trace_token(const char *function, Token_t token, const char *token_str);
void DEBUG_trace_function_entry(const char *function, const char *params);
void DEBUG_trace_function_exit(const char *function, int32_t return_value);

/*
******************************************************************************
Debug Macros for easy use
******************************************************************************
*/
#define DBG_ERROR(cat, fmt, ...)   DEBUG_print(DEBUG_LEVEL_ERROR, cat, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define DBG_WARN(cat, fmt, ...)    DEBUG_print(DEBUG_LEVEL_WARN, cat, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define DBG_INFO(cat, fmt, ...)    DEBUG_print(DEBUG_LEVEL_INFO, cat, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define DBG_TRACE(cat, fmt, ...)   DEBUG_print(DEBUG_LEVEL_TRACE, cat, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define DBG_DETAIL(cat, fmt, ...)  DEBUG_print(DEBUG_LEVEL_DETAIL, cat, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define DBG_ENTER(params)          DEBUG_trace_function_entry(__FUNCTION__, params)
#define DBG_EXIT(ret)              DEBUG_trace_function_exit(__FUNCTION__, ret)
#define DBG_TOKEN(token, str)      DEBUG_trace_token(__FUNCTION__, token, str)
#define DBG_BUFFER(label, buf, len) DEBUG_dump_buffer(label, buf, len)
/*
********************************************************************************
Private variables (static)
********************************************************************************
*/

/*
********************************************************************************
Global variables
********************************************************************************
*/

/*
********************************************************************************
Exported Global variables
********************************************************************************
*/

/*
********************************************************************************
Prototypes of all functions contained in this file (in order of occurrence)
********************************************************************************
*/

#endif  /* DEBUG_H */
