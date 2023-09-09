#ifndef LEXER_H
#define LEXER_H

enum BZL_LX_ERROR 
{
    INVALID_TOKEN = 100,
    ILLEGAL_CHARACTER_IN_NUMBER = 101,
};

enum BZL_LX_TOKEN_TYPE {
    START = -2,
    END_OF_FILE = -1,
    NEWLINE = 0,
    NUMBER = 1,
    IDENT = 2,
    STRING = 3,

    // Keywords
    LABEL = 101,
    GOTO = 102,
    PRINT = 103,
    INPUT = 104,
    LET = 105,
    IF = 106,
    THEN = 107,
    ENDIF = 108,
    WHILE = 109,
    REPEAT = 110,
    ENDWHILE = 111,

    // Operators
    EQ = 201,
    PLUS = 202,
    MINUS = 203,
    ASTERISK = 204,
    SLASH = 205,
    EQEQ = 206,
    NOTEQ = 207,
    LT = 208,
    LTEQ = 209,
    GT = 210,
    GTEQ = 211
};

#define MAX_TOKEN_TEXT_LEN 15

struct bzl_lx_lexer {
    // PUBLIC
    char* source;
    int source_length;

    // PRIVATE
    char current_character;
    int current_position;

    char token_text[MAX_TOKEN_TEXT_LEN];
    int token_type;
};

void bzl_lx_init(struct bzl_lx_lexer* lexer);
void bzl_lx_next_char(struct bzl_lx_lexer* lexer);
char bzl_lx_peek(struct bzl_lx_lexer* lexer);
void bzl_lx_peek_and_assign_token(struct bzl_lx_lexer* lexer, char goal, int token_type, int token_type_default);
void bzl_lx_peek_and_assign_token_err(struct bzl_lx_lexer* lexer, char goal, int token_type);
void bzl_lx_reset_token_text(struct bzl_lx_lexer* lexer);
void bzl_lx_abort(int error);
void bzl_lx_skip_whitespace(struct bzl_lx_lexer* lexer);
void bzl_lx_skip_comment(struct bzl_lx_lexer* lexer);
void bzl_lx_get_token(struct bzl_lx_lexer* lexer);

#endif