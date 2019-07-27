#include "lexer.h"
#include "parser.h"

// According to V `8` is my "life path"
// https://feliciabender.com/eight-life-path-2/
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

    /* 
    struct InstructionList *instruction_list = compile(expression);
#ifdef DEBUG
    display_instruction_list(instruction_list);
#endif
    evaluate(instruction_list);
    */

    return 0;
}
