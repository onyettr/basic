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
    if (!DebugEnabled || DebugLevel < DEBUG_LEVEL_TRACE) {
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
    if (!DebugEnabled || DebugLevel < DEBUG_LEVEL_TRACE) {
        return;
    }
    
    printf("[EXIT]  %s -> %d\n", function, return_value);
}

/*
******************************************************************************
Enhanced versions of existing functions with debug tracing
******************************************************************************
*/

#if 0
/**
 * @brief Enhanced Tokenize_v2 with debug tracing
 */
int32_t Tokenize_v3(char *FileName) {
    FILE *fp = NULL;
    int32_t ErrorCode = SUCCESS;
    int32_t LineNumber = 0;
    
    DBG_ENTER(FileName);
    DBG_INFO(DEBUG_CAT_IO, "Starting tokenization of file: %s", FileName);

    // Input validation
    if (FileName == NULL || *FileName == '\0') {
        DBG_ERROR(DEBUG_CAT_GENERAL, "No filename provided");
        Error("No filename provided");
        DBG_EXIT(ERROR_FILE_NO_FILENAME);
        return ERROR_FILE_NO_FILENAME;
    }

    // File operations
    fp = fopen(FileName, "r");
    if (fp == NULL) {
        DBG_ERROR(DEBUG_CAT_IO, "Failed to open file: %s", FileName);
        Error("Failed to open %s", FileName);
        DBG_EXIT(ERROR_FILE_OPEN_FAILURE);
        return ERROR_FILE_OPEN_FAILURE;
    }
    
    DBG_INFO(DEBUG_CAT_IO, "File opened successfully");

    // Main processing loop
    while (UtilsReadSourceLine(fp, SourceBuffer)) {
        LineNumber++;
        DBG_TRACE(DEBUG_CAT_GENERAL, "Processing line %d", LineNumber);
        DBG_BUFFER("Source Line", SourceBuffer, strlen(SourceBuffer));
        
        ErrorCode = ProcessLine_v2(SourceBuffer, LineNumber);

        if (ErrorCode != SUCCESS) {
            DBG_ERROR(DEBUG_CAT_GENERAL, "Error processing line %d: %s", 
                     LineNumber, error_to_string(ErrorCode));
            Error("Error processing line %d: %s", LineNumber, error_to_string(ErrorCode));
            
            // Continue processing or break based on error severity
            if (ErrorCode == ERROR_FATAL) {
                DBG_ERROR(DEBUG_CAT_GENERAL, "Fatal error encountered, stopping");
                break;
            }
        }
    }

    DBG_INFO(DEBUG_CAT_IO, "Processed %d lines total", LineNumber);

    // Cleanup
    if (fp != NULL) {
        fclose(fp);
        DBG_TRACE(DEBUG_CAT_IO, "File closed");
    }

    // Final EOF token
    TOKEN_print("", TOKEN_EOF);

    // Symbol table cleanup
    if (symTable != NULL) {
        DBG_INFO(DEBUG_CAT_SYMBOLS, "Cleaning up symbol table");
        symbol_table_clean(symTable);
    }

    DBG_EXIT(ErrorCode);
    return ErrorCode;
}

/**
 * @brief Enhanced ProcessLine with debug tracing
 */
int32_t ProcessLine_v2(char *LineBuffer, int32_t LineNumber) {
    char *BufferPtr = NULL;
    Token_t CurrentToken = TOKEN_NO_TOKEN;
    int32_t ErrorCode = SUCCESS;
    int32_t TokenCount = 0;

    DBG_ENTER("LineBuffer, LineNumber");
    DBG_TRACE(DEBUG_CAT_GENERAL, "Processing line %d", LineNumber);

    if (LineBuffer == NULL) {
        DBG_ERROR(DEBUG_CAT_GENERAL, "Null line buffer");
        DBG_EXIT(ERROR_NULL_POINTER);
        return ERROR_NULL_POINTER;
    }

    // Skip leading whitespace
    BufferPtr = UtilsSkipSpaces(LineBuffer);

    // Skip empty lines and comments
    if (*BufferPtr == '\0' || *BufferPtr == '\n') {
        DBG_TRACE(DEBUG_CAT_GENERAL, "Skipping empty line %d", LineNumber);
        DBG_EXIT(SUCCESS);
        return SUCCESS;
    }

    DBG_INFO(DEBUG_CAT_GENERAL, "Line %d: %s", LineNumber, BufferPtr);

    // Process tokens in the line
    while (*BufferPtr != '\0' && ErrorCode == SUCCESS) {
        // Clear token buffer
        memset(TokenBuffer, '\0', sizeof(TokenBuffer));

        // Get next token
        DBG_TRACE(DEBUG_CAT_TOKENIZER, "Getting next token at position %ld", 
                 BufferPtr - LineBuffer);
        CurrentToken = GetNextToken_v2(&BufferPtr, TokenBuffer);

        if (CurrentToken == TOKEN_ERROR) {
            ErrorCode = ERROR_INVALID_TOKEN;
            DBG_ERROR(DEBUG_CAT_TOKENIZER, "Invalid token at line %d, position %ld",
                      LineNumber, BufferPtr - LineBuffer);
            Error("Invalid token at line %d, position %ld",
                  LineNumber, BufferPtr - LineBuffer);
            break;
        }

        // Skip whitespace tokens for cleaner output
        if (CurrentToken == TOKEN_SPACE) {
            continue;
        }

        DBG_TOKEN(CurrentToken, TokenBuffer);

        // Process the token
        ErrorCode = ProcessToken_v2(CurrentToken, TokenBuffer, LineNumber);

        if (Verbose || CurrentToken != TOKEN_SPACE) {
            TOKEN_print(TokenBuffer, CurrentToken);
        }

        TokenCount++;

        // Safety check for runaway tokenization
        if (TokenCount > MAX_TOKENS_PER_LINE) {
            DBG_ERROR(DEBUG_CAT_TOKENIZER, "Too many tokens on line %d (%d tokens)", 
                     LineNumber, TokenCount);
            Error("Too many tokens on line %d", LineNumber);
            ErrorCode = ERROR_TOO_MANY_TOKENS;
            break;
        }
    }

    DBG_INFO(DEBUG_CAT_TOKENIZER, "Line %d processed: %d tokens", LineNumber, TokenCount);
    DBG_EXIT(ErrorCode);
    return ErrorCode;
}

/**
 * @brief Enhanced GetNextToken with debug tracing
 */
Token_t GetNextToken_v2(char **BufferPtr, char *TokenBuffer) {
    char *Bufp = *BufferPtr;
    Token_t TokenType = TOKEN_NO_TOKEN;
    static Token_t PreviousToken = TOKEN_NO_TOKEN;  // Static to maintain context

    DBG_ENTER("BufferPtr, TokenBuffer");
    DBG_DETAIL(DEBUG_CAT_TOKENIZER, "Current character: '%c' (0x%02X)", 
              *Bufp, (unsigned char)*Bufp);

    // Skip whitespace and update pointer
    while (isspace(*Bufp) && *Bufp != '\0') {
        if (*Bufp == ' ' || *Bufp == '\t') {
            Bufp++;
            *BufferPtr = Bufp;
            DBG_TRACE(DEBUG_CAT_TOKENIZER, "Found whitespace, advancing");
            DBG_EXIT(TOKEN_SPACE);
            return TOKEN_SPACE;
        } else if (*Bufp == '\n' || *Bufp == '\r') {
            Bufp++;
            *BufferPtr = Bufp;
            DBG_TRACE(DEBUG_CAT_TOKENIZER, "Found end of line");
            PreviousToken = TOKEN_NO_TOKEN;  // Reset context at line end
            DBG_EXIT(TOKEN_NO_TOKEN);
            return TOKEN_NO_TOKEN;
        }
        Bufp++;
    }

    // End of buffer
    if (*Bufp == '\0') {
        *BufferPtr = Bufp;
        DBG_TRACE(DEBUG_CAT_TOKENIZER, "Reached end of buffer");
        PreviousToken = TOKEN_NO_TOKEN;
        DBG_EXIT(TOKEN_NO_TOKEN);
        return TOKEN_NO_TOKEN;
    }

    // Determine token type and parse accordingly
    if (IsNumericStart_v2(Bufp, PreviousToken)) {
        DBG_TRACE(DEBUG_CAT_TOKENIZER, "Detected numeric token start");
        TokenType = TOKEN_get_number(BufferPtr, TokenBuffer, PreviousToken);
    } else if (isalpha(*Bufp) || *Bufp == '_') {
        DBG_TRACE(DEBUG_CAT_TOKENIZER, "Detected word token start");
        TokenType = TOKEN_get_word(BufferPtr, TokenBuffer);
    } else if (*Bufp == '"' || *Bufp == '\'') {
        DBG_TRACE(DEBUG_CAT_TOKENIZER, "Detected string token start");
        TokenType = TOKEN_get_string(BufferPtr, TokenBuffer);
    } else {
        DBG_TRACE(DEBUG_CAT_TOKENIZER, "Detected special character token");
        TokenType = TOKEN_get_special(BufferPtr, TokenBuffer);
    }

    // Update previous token for context
    PreviousToken = TokenType;
    
    DBG_TOKEN(TokenType, TokenBuffer);
    DBG_EXIT(TokenType);
    return TokenType;
}

/**
 * @brief Enhanced ProcessToken with debug tracing
 */
int32_t ProcessToken_v2(Token_t TokenType, char *TokenString, int32_t LineNumber) {
    int32_t ErrorCode = SUCCESS;
    SymbolTableNode_t *pNewNode = NULL;

    DBG_ENTER("TokenType, TokenString, LineNumber");
    DBG_TRACE(DEBUG_CAT_TOKENIZER, "Processing token: %s = '%s' at line %d",
             TOKEN_type_to_string(TokenType), TokenString, LineNumber);

    switch (TokenType) {
        case TOKEN_WORD:
            if (is_direct_command(TokenString)) {
                DBG_INFO(DEBUG_CAT_TOKENIZER, "Found direct command: %s", TokenString);
                TokenType = TOKEN_direct_command(TokenString);
                ErrorCode = TOKEN_execute_direct_command(TokenType, TokenString);
            } else if (is_direct_keyword(TokenString)) {
                DBG_INFO(DEBUG_CAT_TOKENIZER, "Found keyword: %s", TokenString);
                if (Verbose) {
                    printf("Keyword found: %s\n", TokenString);
                }
            } else {
                DBG_TRACE(DEBUG_CAT_SYMBOLS, "Processing identifier: %s", TokenString);
                // Handle identifier
                pNewNode = symbol_table_search(TokenString, symTable);
                if (pNewNode == NULL) {
                    DBG_INFO(DEBUG_CAT_SYMBOLS, "Adding new symbol: %s", TokenString);
                    pNewNode = symbol_table_add_node(TokenString, &symTable);
                    if (pNewNode == NULL) {
                        DBG_ERROR(DEBUG_CAT_SYMBOLS, 
                                 "Failed to add symbol '%s' to table at line %d",
                                 TokenString, LineNumber);
                        Error("Failed to add symbol '%s' to table at line %d",
                              TokenString, LineNumber);
                        ErrorCode = ERROR_SYMBOL_TABLE_FULL;
                    }
                } else {
                    DBG_TRACE(DEBUG_CAT_SYMBOLS, "Symbol already exists: %s", TokenString);
                }
                TokenType = TOKEN_IDENTIFIER;
            }
            break;

        case TOKEN_DIGIT:
            DBG_TRACE(DEBUG_CAT_TOKENIZER, "Processing numeric literal");
            break;

        case TOKEN_STRING:
            DBG_TRACE(DEBUG_CAT_TOKENIZER, "Processing string literal");
            break;

        case TOKEN_ERROR:
            DBG_ERROR(DEBUG_CAT_TOKENIZER, "Token error encountered");
            ErrorCode = ERROR_INVALID_TOKEN;
            break;

        default:
            DBG_DETAIL(DEBUG_CAT_TOKENIZER, "Processing standard token");
            break;
    }

    DBG_EXIT(ErrorCode);
    return ErrorCode;
}

/**
 * @brief Enhanced IsNumericStart with debug tracing
 */
bool IsNumericStart_v2(char *BufferPtr, Token_t PreviousToken) {
    bool result = false;
    
    if (isdigit(*BufferPtr)) {
        result = true;
        DBG_DETAIL(DEBUG_CAT_TOKENIZER, "Numeric start: digit '%c'", *BufferPtr);
    } else if ((*BufferPtr == '-' || *BufferPtr == '.') && isdigit(*(BufferPtr + 1))) {
        result = true;
        DBG_DETAIL(DEBUG_CAT_TOKENIZER, "Numeric start: '%c' followed by digit", *BufferPtr);
    } else if (*BufferPtr == '.' && (PreviousToken == TOKEN_DIGIT || isdigit(*(BufferPtr + 1)))) {
        result = true;
        DBG_DETAIL(DEBUG_CAT_TOKENIZER, "Numeric start: decimal point with context");
    }
    
    return result;
}
#endif
