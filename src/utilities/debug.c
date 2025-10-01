/**
 * @brief Debug and tracing functions for tokenizer
 */

/*
********************************************************************************
Includes
********************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include "basic.h"
#include "tokenizer.h"
#include "debug.h"

/*
******************************************************************************
Debug Variables and Macros
******************************************************************************
*/
static bool DebugEnabled = false;
static int32_t DebugLevel = 0;

/*
******************************************************************************
Debug Function Implementations
******************************************************************************
*/

/**
 * @brief Initialize debug system
 * @param enable - true to enable debug output
 * @param level - debug level (0-5)
 */
void DEBUG_init(bool enable, int32_t level) {
    DebugEnabled = enable;
    DebugLevel = level;
    
    if (DebugEnabled) {
        printf("[DEBUG] Debug system initialized - Level: %d\n", level);
    }
}

/**
 * @brief Set debug level
 * @param level - new debug level
 */
void DEBUG_set_level(int32_t level) {
    if (level >= DEBUG_LEVEL_OFF && level <= DEBUG_LEVEL_DETAIL) {
        DebugLevel = level;
        if (DebugEnabled) {
            printf("[DEBUG] Debug level set to %d\n", level);
        }
    }
}

/**
 * @brief Set debug categories
 * @param categories - bitmask of categories to enable
 */
void DEBUG_set_categories(uint32_t categories) {
    DebugCategories = categories;
    if (DebugEnabled) {
        printf("[DEBUG] Debug categories set to 0x%08X\n", categories);
    }
}

/**
 * @brief Enable specific debug category
 * @param category - category to enable
 */
void DEBUG_enable_category(uint32_t category) {
    DebugCategories |= category;
}

/**
 * @brief Disable specific debug category
 * @param category - category to disable
 */
void DEBUG_disable_category(uint32_t category) {
    DebugCategories &= ~category;
}

/**
 * @brief Core debug print function
 * @param level - debug level of this message
 * @param category - category bitmask
 * @param function - calling function name
 * @param line - line number
 * @param format - printf-style format string
 */
void DEBUG_print(int32_t level, uint32_t category, const char *function, 
                 int32_t line, const char *format, ...) {
    va_list args;
    char timestamp[32];
    time_t now;
    struct tm *tm_info;
    
    // Check if debug is enabled and level/category match
    if (!DebugEnabled || level > DebugLevel || !(DebugCategories & category)) {
        return;
    }
    
    // Get timestamp
    time(&now);
    tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", tm_info);
    
    // Print debug header
    const char *level_str[] = {"OFF", "ERR", "WRN", "INF", "TRC", "DTL"};
    printf("[%s] [%s] %s:%d - ", timestamp, level_str[level], function, line);
    
    // Print the actual message
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
    fflush(stdout);  // Ensure immediate output
}

/**
 * @brief Dump buffer contents in hex and ASCII
 * @param label - descriptive label for the buffer
 * @param buffer - buffer to dump
 * @param length - number of bytes to dump
 */
void DEBUG_dump_buffer(const char *label, const char *buffer, size_t length) {
    if (!DebugEnabled || DebugLevel < DEBUG_LEVEL_DETAIL) {
        return;
    }

    printf("[DEBUG DUMP] %s (%zu bytes):\n", label, length);

    for (size_t i = 0; i < length; i += 16) {
        // Print offset
        printf("%04zX: ", i);

        // Print hex bytes
        for (size_t j = 0; j < 16 && (i + j) < length; j++) {
            printf("%02X ", (unsigned char)buffer[i + j]);
        }
        
        // Pad if less than 16 bytes
        for (size_t j = length - i; j < 16 && i + j >= length; j++) {
            printf("   ");
        }
        
        printf(" |");
        
        // Print ASCII representation
        for (size_t j = 0; j < 16 && (i + j) < length; j++) {
            char c = buffer[i + j];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        
        printf("|\n");
    }
    
    printf("\n");
}

/**
 * @brief Trace token creation/processing
 * @param function - calling function
 * @param token - token type
 * @param token_str - token string value
 */
void DEBUG_trace_token(const char *function, Token_t token, const char *token_str) {
    if (!DebugEnabled || DebugLevel < DEBUG_LEVEL_TRACE || 
        !(DebugCategories & DEBUG_CAT_TOKENIZER)) {
        return;
    }
    
    printf("[TOKEN] %s: %s = '%s'\n", 
           function, 
           TOKEN_type_to_string(token), 
           token_str ? token_str : "<null>");
}

/**
 * @brief Trace function entry
 * @param function - function name
 * @param params - parameter description
 */
void DEBUG_trace_function_entry(const char *function, const char *params) {
    if (!DebugEnabled || DebugLevel <= DEBUG_LEVEL_TRACE) {
        return;
    }

    printf("[ENTER] %s(%s)\n", function, params ? params : "");
}

/**
 * @brief Trace function exit
 * @param function - function name
 * @param return_value - return value
 */
void DEBUG_trace_function_exit(const char *function, int32_t return_value) {
    if (!DebugEnabled || DebugLevel <= DEBUG_LEVEL_TRACE) {
        return;
    }

    printf("[EXIT]  %s -> %d\n", function, return_value);
}
