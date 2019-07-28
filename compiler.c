#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "parser.h"
#include "utils.h"

void push_procedure_call_context(struct ProcedureCall *procedure_call, struct ContextStack *context_stack)
{
    struct Context *operator_context;
    MALLOC(operator_context, sizeof(struct Context));

    operator_context->expression = procedure_call->operator_;
    push_context(operator_context, context_stack);

    // XXX: Push them on backwards for LtoR evaluation
    struct Operand *operand = procedure_call->operand_list->tail;

    while (operand->prev != NULL)
    {
        struct Context *operand_context;
        MALLOC(operand_context, sizeof(struct Context));

        operand_context->expression = operand->expression;
        push_context(operand_context, context_stack);
    }
}

struct InstructionList *compile(struct Expression *expression)
{
    struct ContextStack *context_stack;
    struct InstructionList *instruction_list;

    MALLOC(context_stack, sizeof(struct ContextStack));
    MALLOC(instruction_list, sizeof(struct InstructionList));

    struct Context *context;

    MALLOC(context, sizeof(struct Context));
    context->expression = expression;

    push_context(context, context_stack);

    while (context_stack->top != NULL)
    {
        compile_expression(context_stack, instruction_list);
    }

    // FIXME: For now, assume that we always want the last value on the stack
    struct Instruction *instruction;
    MALLOC(instruction, sizeof(struct Instruction));
    instruction->kind = INSTRUCTION_POP;
    instruction->operand = NULL;
    add_instruction(instruction, instruction_list);

    return instruction_list;
}

void *compile_expression(struct ContextStack *context_stack,
                         struct InstructionList *instruction_list)
{
    struct Instruction *instruction;
    struct Context *context = pop_context(context_stack);
    struct Expression *expression = context->expression;

    switch (expression->kind)
    {
    case EXPR_IDENTIFIER:
        if (strcmp(expression->identifier->name, "+") == 0)
        {
            MALLOC(instruction, sizeof(struct Instruction));
            instruction->kind = INSTRUCTION_ADD;
            instruction->operand = NULL;
            add_instruction(instruction, instruction_list);
        }
        else if (strcmp(expression->identifier->name, "*") == 0)
        {
            MALLOC(instruction, sizeof(struct Instruction));
            instruction->kind = INSTRUCTION_MUL;
            instruction->operand = NULL;
            add_instruction(instruction, instruction_list);
        }
        else
        {
            fprintf(stderr, "Cannot compile identifier: %s", expression->identifier->name);
            exit(1);
        }
        // TODO: Non-builtins
        break;
    case EXPR_LITERAL:
        switch (expression->literal->kind)
        {
        case LIT_NUMBER:
            MALLOC(instruction, sizeof(struct Instruction));
            MALLOC(instruction->operand, sizeof(union RuntimeValue));
            instruction->kind = INSTRUCTION_PUSH;
            instruction->operand->immediate = expression->literal->number;
            add_instruction(instruction, instruction_list);
            break;
        }
    case EXPR_PROCEDURE_CALL:
        push_procedure_call_context(expression->procedure_call, context_stack);
        break;
    }
}
