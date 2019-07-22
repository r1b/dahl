#include "lexer.h"

int main(int argc, char** argv) {
    struct TokenList* token_list = lex(stdin);
    display_token_list(token_list);
    return 0;
}
