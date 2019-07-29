#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "vm.h"

// According to V `8` is my "life path"
// https://feliciabender.com/eight-life-path-2/
#define DEBUG 8

int main(int argc, char **argv)
{
    struct TokenList *token_list = lex(stdin);
    struct Expression *expression = parse(token_list);

    struct InstructionList *instruction_list = compile(expression);
    union RuntimeValue *runtime_value = vm_exec(instruction_list);

    fprintf(stdout, "God says: %d", runtime_value->immediate);

    return 0;
}
