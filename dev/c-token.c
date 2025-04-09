#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Token types
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_COMMENT,
    TOKEN_WHITESPACE,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

// Tokenizer structure
typedef struct {
    const char* source;
    size_t source_len;
    size_t position;
    size_t line;
    size_t column;
    char current_char;
} Tokenizer;

// Array of tokens
typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenArray;

// List of keywords
const char* keywords[] = {
    "if", "else", "while", "for", "return", "break", "continue",
    "int", "float", "char", "void", "struct", "const", "static",
    "switch", "case", "default", "do", "typedef", "enum", "sizeof",
    NULL  // Sentinel value
};

// Initialize a tokenizer with source code
Tokenizer init_tokenizer(const char* source) {
    Tokenizer tokenizer;
    tokenizer.source = source;
    tokenizer.source_len = strlen(source);
    tokenizer.position = 0;
    tokenizer.line = 1;
    tokenizer.column = 1;
    tokenizer.current_char = source[0];
    return tokenizer;
}

// Initialize token array
TokenArray init_token_array() {
    TokenArray array;
    array.capacity = 16;
    array.count = 0;
    array.tokens = malloc(array.capacity * sizeof(Token));
    if (!array.tokens) {
        fprintf(stderr, "Failed to allocate memory for tokens\n");
        exit(EXIT_FAILURE);
    }
    return array;
}

// Add token to array (with automatic resize)
void add_token(TokenArray* array, Token token) {
    if (array->count >= array->capacity) {
        array->capacity *= 2;
        Token* new_tokens = realloc(array->tokens, array->capacity * sizeof(Token));
        if (!new_tokens) {
            fprintf(stderr, "Failed to allocate memory for tokens\n");
            exit(EXIT_FAILURE);
        }
        array->tokens = new_tokens;
    }
    array->tokens[array->count++] = token;
}

// Free token array
void free_token_array(TokenArray* array) {
    for (size_t i = 0; i < array->count; i++) {
        free(array->tokens[i].value);
    }
    free(array->tokens);
    array->tokens = NULL;
    array->count = 0;
    array->capacity = 0;
}

// Helper function to advance the tokenizer
void advance_tokenizer(Tokenizer* tokenizer) {
    if (tokenizer->position < tokenizer->source_len) {
        tokenizer->position++;
        
        if (tokenizer->current_char == '\n') {
            tokenizer->line++;
            tokenizer->column = 1;
        } else {
            tokenizer->column++;
        }
        
        tokenizer->current_char = tokenizer->position < tokenizer->source_len ? 
                                tokenizer->source[tokenizer->position] : '\0';
    }
}

// Helper function to peek ahead
char peek(Tokenizer* tokenizer, int offset) {
    size_t position = tokenizer->position + offset;
    return position < tokenizer->source_len ? tokenizer->source[position] : '\0';
}

// Check if a string is a keyword
bool is_keyword(const char* str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Create a new token
Token create_token(TokenType type, const char* value, int line, int column) {
    Token token;
    token.type = type;
    token.value = strdup(value);
    token.line = line;
    token.column = column;
    return token;
}

// Process identifier or keyword
Token process_identifier(Tokenizer* tokenizer) {
    size_t start_pos = tokenizer->position;
    int start_column = tokenizer->column;
    
    // Read all alphanumeric characters and underscores
    while (isalnum(tokenizer->current_char) || tokenizer->current_char == '_') {
        advance_tokenizer(tokenizer);
    }
    
    // Extract the identifier
    size_t length = tokenizer->position - start_pos;
    char* value = malloc(length + 1);
    if (!value) {
        fprintf(stderr, "Failed to allocate memory for token value\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(value, tokenizer->source + start_pos, length);
    value[length] = '\0';
    
    // Check if it's a keyword
    TokenType type = is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    
    Token token = {type, value, tokenizer->line, start_column};
    return token;
}

// Process number (integer or float)
Token process_number(Tokenizer* tokenizer) {
    size_t start_pos = tokenizer->position;
    int start_column = tokenizer->column;
    bool has_decimal = false;
    
    // Read all digits and possibly a decimal point
    while (isdigit(tokenizer->current_char) || (!has_decimal && tokenizer->current_char == '.')) {
        if (tokenizer->current_char == '.') {
            has_decimal = true;
        }
        advance_tokenizer(tokenizer);
    }
    
    // Extract the number
    size_t length = tokenizer->position - start_pos;
    char* value = malloc(length + 1);
    if (!value) {
        fprintf(stderr, "Failed to allocate memory for token value\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(value, tokenizer->source + start_pos, length);
    value[length] = '\0';
    
    Token token = {TOKEN_NUMBER, value, tokenizer->line, start_column};
    return token;
}

// Process string literal
Token process_string(Tokenizer* tokenizer) {
    size_t start_pos = tokenizer->position;
    int start_column = tokenizer->column;
    char quote_char = tokenizer->current_char;
    
    advance_tokenizer(tokenizer); // Skip the opening quote
    
    // Read until closing quote
    while (tokenizer->current_char != '\0' && tokenizer->current_char != quote_char) {
        // Handle escape sequences
        if (tokenizer->current_char == '\\' && peek(tokenizer, 1) == quote_char) {
            advance_tokenizer(tokenizer); // Skip the backslash
        }
        advance_tokenizer(tokenizer);
    }
    
    if (tokenizer->current_char == quote_char) {
        advance_tokenizer(tokenizer); // Skip the closing quote
    }
    
    // Extract the string including quotes
    size_t length = tokenizer->position - start_pos;
    char* value = malloc(length + 1);
    if (!value) {
        fprintf(stderr, "Failed to allocate memory for token value\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(value, tokenizer->source + start_pos, length);
    value[length] = '\0';
    
    Token token = {TOKEN_STRING, value, tokenizer->line, start_column};
    return token;
}

// Process operator
Token process_operator(Tokenizer* tokenizer) {
    size_t start_pos = tokenizer->position;
    int start_column = tokenizer->column;
    
    // Check for multi-character operators
    char first_char = tokenizer->current_char;
    char next_char = peek(tokenizer, 1);
    
    // Two-character operators
    if ((first_char == '+' && next_char == '+') ||
        (first_char == '-' && next_char == '-') ||
        (first_char == '=' && next_char == '=') ||
        (first_char == '!' && next_char == '=') ||
        (first_char == '<' && next_char == '=') ||
        (first_char == '>' && next_char == '=') ||
        (first_char == '&' && next_char == '&') ||
        (first_char == '|' && next_char == '|') ||
        (first_char == '+' && next_char == '=') ||
        (first_char == '-' && next_char == '=') ||
        (first_char == '*' && next_char == '=') ||
        (first_char == '/' && next_char == '=') ||
        (first_char == '%' && next_char == '=') ||
        (first_char == '-' && next_char == '>')) {
        advance_tokenizer(tokenizer);
        advance_tokenizer(tokenizer);
    } else {
        advance_tokenizer(tokenizer);
    }
    
    // Extract the operator
    size_t length = tokenizer->position - start_pos;
    char* value = malloc(length + 1);
    if (!value) {
        fprintf(stderr, "Failed to allocate memory for token value\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(value, tokenizer->source + start_pos, length);
    value[length] = '\0';
    
    Token token = {TOKEN_OPERATOR, value, tokenizer->line, start_column};
    return token;
}

// Process line comment
Token process_line_comment(Tokenizer* tokenizer) {
    size_t start_pos = tokenizer->position;
    int start_column = tokenizer->column;
    
    // Skip the '//'
    advance_tokenizer(tokenizer);
    advance_tokenizer(tokenizer);
    
    // Read until end of line or file
    while (tokenizer->current_char != '\0' && tokenizer->current_char != '\n') {
        advance_tokenizer(tokenizer);
    }
    
    // Extract the comment
    size_t length = tokenizer->position - start_pos;
    char* value = malloc(length + 1);
    if (!value) {
        fprintf(stderr, "Failed to allocate memory for token value\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(value, tokenizer->source + start_pos, length);
    value[length] = '\0';
    
    Token token = {TOKEN_COMMENT, value, tokenizer->line, start_column};
    return token;
}

// Process block comment
Token process_block_comment(Tokenizer* tokenizer) {
    size_t start_pos = tokenizer->position;
    int start_column = tokenizer->column;
    
    // Skip the '/*'
    advance_tokenizer(tokenizer);
    advance_tokenizer(tokenizer);
    
    // Read until '*/' or end of file
    while (tokenizer->current_char != '\0' && 
           !(tokenizer->current_char == '*' && peek(tokenizer, 1) == '/')) {
        advance_tokenizer(tokenizer);
    }
    
    // Skip the '*/' if found
    if (tokenizer->current_char != '\0') {
        advance_tokenizer(tokenizer);
        advance_tokenizer(tokenizer);
    }
    
    // Extract the comment
    size_t length = tokenizer->position - start_pos;
    char* value = malloc(length + 1);
    if (!value) {
        fprintf(stderr, "Failed to allocate memory for token value\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(value, tokenizer->source + start_pos, length);
    value[length] = '\0';
    
    Token token = {TOKEN_COMMENT, value, tokenizer->line, start_column};
    return token;
}

// Process whitespace
Token process_whitespace(Tokenizer* tokenizer) {
    size_t start_pos = tokenizer->position;
    int start_column = tokenizer->column;
    
    // Skip all whitespace
    while (isspace(tokenizer->current_char)) {
        advance_tokenizer(tokenizer);
    }
    
    // Extract the whitespace
    size_t length = tokenizer->position - start_pos;
    char* value = malloc(length + 1);
    if (!value) {
        fprintf(stderr, "Failed to allocate memory for token value\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(value, tokenizer->source + start_pos, length);
    value[length] = '\0';
    
    Token token = {TOKEN_WHITESPACE, value, tokenizer->line, start_column};
    return token;
}

// Get the next token
Token get_next_token(Tokenizer* tokenizer) {
    if (tokenizer->position >= tokenizer->source_len) {
        return create_token(TOKEN_EOF, "", tokenizer->line, tokenizer->column);
    }
    
    // Skip whitespace
    if (isspace(tokenizer->current_char)) {
        return process_whitespace(tokenizer);
    }
    
    // Identifier or keyword
    if (isalpha(tokenizer->current_char) || tokenizer->current_char == '_') {
        return process_identifier(tokenizer);
    }
    
    // Number
    if (isdigit(tokenizer->current_char)) {
        return process_number(tokenizer);
    }
    
    // String literal
    if (tokenizer->current_char == '"' || tokenizer->current_char == '\'') {
        return process_string(tokenizer);
    }
    
    // Comments
    if (tokenizer->current_char == '/' && peek(tokenizer, 1) == '/') {
        return process_line_comment(tokenizer);
    }
    
    if (tokenizer->current_char == '/' && peek(tokenizer, 1) == '*') {
        return process_block_comment(tokenizer);
    }
    
    // Delimiters (punctuation)
    if (strchr("()[]{};,.", tokenizer->current_char)) {
        char delimiter[2] = {tokenizer->current_char, '\0'};
        Token token = create_token(TOKEN_DELIMITER, delimiter, tokenizer->line, tokenizer->column);
        advance_tokenizer(tokenizer);
        return token;
    }
    
    // Operators
    if (strchr("+-*/%=<>!&|^~?:", tokenizer->current_char)) {
        return process_operator(tokenizer);
    }
    
    // Unknown character
    char unknown[2] = {tokenizer->current_char, '\0'};
    Token token = create_token(TOKEN_UNKNOWN, unknown, tokenizer->line, tokenizer->column);
    advance_tokenizer(tokenizer);
    return token;
}

// Tokenize the entire source code
TokenArray tokenize_source(const char* source) {
    Tokenizer tokenizer = init_tokenizer(source);
    TokenArray tokens = init_token_array();
    
    Token token;
    do {
        token = get_next_token(&tokenizer);
        // Skip whitespace tokens if not needed
        // if (token.type != TOKEN_WHITESPACE) {
            add_token(&tokens, token);
        // } else {
        //     free(token.value);
        // }
    } while (token.type != TOKEN_EOF);
    
    return tokens;
}

// Get token type name
const char* get_token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_DELIMITER: return "DELIMITER";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_WHITESPACE: return "WHITESPACE";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "???";
    }
}

int main() {
    // Example C code to tokenize
    const char* source_code = 
        "// This is a sample C program\n"
        "/* Multiline\n"
        "   Comment */\n"
        "#include <stdio.h>\n\n"
        "int main() {\n"
        "    // Print a greeting\n"
        "    int x = 42;\n"
        "    float pi = 3.14159;\n"
        "    char *message = \"Hello, World!\";\n"
        "    \n"
        "    if (x > 0) {\n"
        "        printf(\"%s\\n\", message);\n"
        "    }\n"
        "    \n"
        "    return 0;\n"
        "}\n";
    
    printf("Tokenizing the following source code:\n\n%s\n", source_code);
    
    TokenArray tokens = tokenize_source(source_code);
    
    printf("\nTokens:\n");
    printf("%-5s %-12s %-15s %-10s %-10s\n", "No.", "Type", "Value", "Line", "Column");
    printf("--------------------------------------------------\n");
    
    for (size_t i = 0; i < tokens.count; i++) {
        Token token = tokens.tokens[i];
        
        // Skip whitespace tokens in the output
        if (token.type == TOKEN_WHITESPACE) {
            continue;
        }
        
        // Format the token value for display (truncate if too long)
        char value_display[16] = {0};
        if (strlen(token.value) > 15) {
            strncpy(value_display, token.value, 12);
            strcat(value_display, "...");
        } else {
            strcpy(value_display, token.value);
        }
        
        printf("%-5zu %-12s %-15s %-10d %-10d\n", 
               i + 1, 
               get_token_type_name(token.type),
               value_display,
               token.line,
               token.column);
    }
    
    printf("\nFound %zu tokens (excluding whitespace).\n", tokens.count);
    
    free_token_array(&tokens);
    return 0;
}
