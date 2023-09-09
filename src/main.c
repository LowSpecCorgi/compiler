// STDL
#include "stdio.h"

// USR
#include "lexer.h"

int main(void) {
    char* source = "++-*=+= 21201 5124 == #lol\n =!=>=<= hello my gees";
    
    struct bzl_lx_lexer lexer = {
        .source=source,
    };

    printf("\n***\nSTARTING LEXER\nWITH SOURCE: %s\n***\n\n", source);

    bzl_lx_init(&lexer);
    

    while(lexer.token_type != END_OF_FILE) {
        printf("TOKEN_TYPE: %d, N: %d, OPT_TXT: %s\n", lexer.token_type, lexer.current_position, lexer.token_text);

        bzl_lx_get_token(&lexer);
    }

    printf("\n***\nLEXER SUCCESFULL\n***\n\n", source);

    return 0;
}