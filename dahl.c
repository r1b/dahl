#include "lexer.h"
#include "parser.h"

int main(int argc, char** argv) {
    struct TokenList* token_list = lex(stdin);
    display_token_list(token_list);

    struct Expression* expression = parse(token_list);
    display_expression(expression);

    return 0;
}
