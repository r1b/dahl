#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "vm.h"

int main(void) {
    struct TokenList *token_list = lex(stdin);
    struct Expression *expression = parse(token_list);
    struct InstructionList *instruction_list = compile(expression);
    union RuntimeValue *runtime_value = vm_exec(instruction_list);

    fprintf(stdout, "God says: %d\n", runtime_value->immediate);

    return 0;
}
