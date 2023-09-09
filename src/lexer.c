// STDL
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"

// USR
#include "lexer.h"

void bzl_lx_init(struct bzl_lx_lexer* lexer) {
    // The newline simplifies parsing the last token
    lexer->current_character = ' ';
    lexer->current_position = -1;
    lexer->token_type = START;

    int size = 0;
    char pos = ' ';
    while (pos != '\0') {
        size ++;
        pos = lexer->source[size];
    }

    lexer->source_length = size;

    bzl_lx_next_char(lexer);
}

void bzl_lx_next_char(struct bzl_lx_lexer* lexer) {
    lexer->current_position ++;

    if (lexer->current_position >= lexer->source_length) {
        lexer->current_character = '\0';
    } else {
        lexer->current_character = lexer->source[lexer->current_position];
    }
}

char bzl_lx_peek(struct bzl_lx_lexer* lexer) {
    if (lexer->current_position + 1 >= lexer->source_length) return '\0';

    return lexer->source[lexer->current_position + 1];
}

void bzl_lx_skip_whitespace(struct bzl_lx_lexer* lexer) {
    while (lexer->current_character == ' ' || lexer->current_character == '\t' || lexer->current_character == '\r') bzl_lx_next_char(lexer);
}

void bzl_lx_skip_comment(struct bzl_lx_lexer* lexer) {
    if (lexer->current_character == '#') {
        while (lexer->current_character != '\n') bzl_lx_next_char(lexer);
    }
}

void bzl_lx_peek_and_assign_token_err(struct bzl_lx_lexer* lexer, char goal, int token_type) {
    
    if (bzl_lx_peek(lexer) == goal) {
        lexer->token_type = token_type;
        bzl_lx_next_char(lexer);
    } else {
        bzl_lx_abort(INVALID_TOKEN);
    }
}


void bzl_lx_peek_and_assign_token(struct bzl_lx_lexer* lexer, char goal, int token_type, int token_type_default) {
    if (bzl_lx_peek(lexer) == goal) {
        lexer->token_type = token_type;
        bzl_lx_next_char(lexer);
    } else {
        lexer->token_type = token_type_default;
    }
}

void bzl_lx_abort(int error) {
    char* message;

    switch (error)
    {
    case INVALID_TOKEN:
        message = "AN INVALID TOKEN WAS PROVIDED";
        break;
    
    default:
        message = "UNKOWN MESSAGE";
        break;
    }
    
    printf("\n***\nLEXER ERROR: %s\nABORTING...\n***\n\n", message);

    exit(1);
}

void bzl_lx_reset_token_text(struct bzl_lx_lexer* lexer) {
    for (int i = 0; i < MAX_TOKEN_TEXT_LEN + 1; i++) {
        lexer->token_text[i] = '\0';
    }
}

void bzl_lx_get_token(struct bzl_lx_lexer* lexer) {
    bzl_lx_skip_whitespace(lexer);
    bzl_lx_skip_comment(lexer);

    switch (lexer->current_character)
    {
        case '+':
            lexer->token_type = PLUS;
            break;
        case '-':
            lexer->token_type = MINUS;
            break;
        case '*':
            lexer->token_type = ASTERISK;
            break;
        case '/':
            lexer->token_type = SLASH;
            break;
        case '\n':
            lexer->token_type = NEWLINE;
            break;
        case '\0':
            lexer->token_type = END_OF_FILE;
            break;
        // 2 character
        case '=': 
            bzl_lx_peek_and_assign_token(lexer, '=', EQEQ, EQ);
            break;
        case '>': 
            bzl_lx_peek_and_assign_token(lexer, '=', GTEQ, GT);
            break;
        case '<': 
            bzl_lx_peek_and_assign_token(lexer, '=', LTEQ, LT);
            break;
        case '!':
            bzl_lx_peek_and_assign_token_err(lexer, '=', NOTEQ);
            break;
        case '\"':
            bzl_lx_peek_and_assign_token_err(lexer, '\"', STRING);
            break;
        default:
            // Check for number
            if (isdigit(lexer->current_character)) {
                int start_pos = lexer->current_position;

                while (isdigit(bzl_lx_peek(lexer))) {
                    bzl_lx_next_char(lexer);
                }

                if (bzl_lx_peek(lexer) == '.') {
                    bzl_lx_next_char(lexer);

                    if (!isdigit(bzl_lx_peek(lexer))) {
                        bzl_lx_abort(ILLEGAL_CHARACTER_IN_NUMBER);
                    }

                    while (isdigit(bzl_lx_peek(lexer))) {
                        bzl_lx_next_char(lexer);
                    }
                }

                bzl_lx_reset_token_text(lexer);

                int j = 0;
                for (int i = start_pos; i < lexer->current_position + 1; i++) {
                    lexer->token_text[j] = lexer->source[i];
                    j++;
                }
                lexer->token_text[j + 1] = '\0';

                lexer->token_type = NUMBER;

                break;
            }

            // Check for identifier
            if (isalpha(lexer->current_character)) {
                int start_pos = lexer->current_position;

                while (isalpha(bzl_lx_peek(lexer))) {
                    bzl_lx_next_char(lexer);
                }

                bzl_lx_reset_token_text(lexer);

                // TODO: Chuck in function 
                int j = 0;
                for (int i = start_pos; i < lexer->current_position + 1; i++) {
                    lexer->token_text[j] = lexer->source[i];
                    j++;
                }
                lexer->token_text[j + 1] = '\0';
                lexer->token_type = IDENT;

                // TODO: Keyword checking hould be done here

                break;
            }
            
            bzl_lx_abort(INVALID_TOKEN); // UNKNOWN TOKEN
    }

    bzl_lx_next_char(lexer);
}