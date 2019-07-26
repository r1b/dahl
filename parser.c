#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "utils.h"

void init_context_stack(struct ContextStack* context_stack) {
    MALLOC(context_stack, sizeof(struct ContextStack));
    context_stack->top = NULL;
    context_stack->bottom = NULL;
}

void init_context(struct Context** context) {
    MALLOC(*context, sizeof(struct Context));
    MALLOC((*context)->expression, sizeof(struct Expression));
    MALLOC((*context)->prev, sizeof(struct Context));
    MALLOC((*context)->next, sizeof(struct Context));
}

void push_context(struct Context* context, struct ContextStack* context_stack) {
    context->prev = context_stack->top;
    context->next = NULL;

    context_stack->top = context;

    if (context_stack->bottom == NULL) {
        context_stack->bottom = context;
    }
}

struct Context* pop_context(struct ContextStack* context_stack) {
    struct Context* context = context_stack->top;
    context_stack->top = context->prev;

    if (context_stack->top == NULL) {
        context_stack->bottom = NULL;
    }

    return context;
}

void update_procedure_call(struct Expression* expression, struct ProcedureCall* procedure_call) {
    if (procedure_call == NULL) {
        MALLOC(procedure_call, sizeof(struct ProcedureCall));
        MALLOC(procedure_call->operator, sizeof(struct Expression));
        MALLOC(procedure_call->operand_list, sizeof(struct OperandList));

        procedure_call->operator = expression;
        procedure_call->operand_list = NULL;
        return;
    }

    struct Operand* operand;
    MALLOC(operand, sizeof(struct Operand*));

    if (procedure_call->operand_list == NULL) {
        MALLOC(procedure_call->operand_list, sizeof(struct OperandList));

        operand->value = expression;
        operand->prev = NULL;
        operand->next = NULL;

        procedure_call->operand_list->head = operand;
        procedure_call->operand_list->tail = operand;
        return;
    }

    operand->value = expression;
    operand->prev = procedure_call->operand_list->head;
    operand->next = NULL;

    procedure_call->operand_list->head = operand;
}

void update_expression(struct Expression* src_expression, struct Expression* dest_expression) {
    switch (dest_expression->kind) {
        case PROCEDURE_CALL:
            update_procedure_call(src_expression, dest_expression->procedure_call);
            break;
        default:
            fprintf(stderr, "Cannot update expression: %d", dest_expression->kind);
            exit(1);
    }
}

struct Expression* parse(struct TokenList* token_list) {
    struct ContextStack context_stack;
    struct Expression* expression = NULL;
    struct Token* token;

    token = token_list->head;
    init_context_stack(&context_stack);

    while (token != NULL) {
        expression = parse_expression(token, &context_stack);
        token = token->next;
    }

    return expression;
}

struct Expression* parse_expression(struct Token* token, struct ContextStack* context_stack) {
    struct Context* context = NULL;
    struct Expression* expression;

    MALLOC(expression, sizeof(struct Expression));

    switch (token->kind) {
        case IDENTIFIER:
            expression->kind = IDENT;
            MALLOC(expression->identifier, sizeof(struct Identifier));
            memcpy(&token->lexeme, &expression->identifier->name, strlen((const char*)token->lexeme) + 1);
            break;
        case NUMBER:
            expression->kind = LITERAL;
            MALLOC(expression->literal, sizeof(struct Literal));
            expression->literal->kind = NUM;
            // FIXME: This won't work for numeric tower
            expression->literal->number = atoi((const char*)token->lexeme);
            break;
        case PAREN_L:
            MALLOC(expression->procedure_call, sizeof(struct ProcedureCall));

            expression->kind = PROCEDURE_CALL;
            expression->procedure_call = NULL;

            init_context(&context);
            context->expression = expression;

            push_context(context, context_stack);
            break;
        case PAREN_R:
            context = pop_context(context_stack);
            expression = context->expression;
            break;
        default:
            fprintf(stderr, "Unrecognized token kind: %d", (int)token->kind);
            exit(1);
    }

    if (context_stack->top != NULL) {
        update_expression(expression, context_stack->top->expression);
    }

    return expression;
}

void display_expression(struct Expression* expression) {
    fprintf(stdout, "TODO");
}
