#include "lexer.h"
#include "parser.h"

#define DEBUG 8

int main(int argc, char **argv)
{
    struct TokenList *token_list = lex(stdin);
#ifdef DEBUG
    display_token_list(token_list);
#endif

    struct Expression *expression = parse(token_list);
#ifdef DEBUG
    display_expression(expression);
#endif

    return 0;
}
