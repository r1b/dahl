#include "parser.h"
#include "lexer.h"
#include "utils.h"

struct Expression *create_identifier(struct Token *token) {
    struct Expression *expression;
    MALLOC(expression, sizeof(struct Expression));

    expression->kind = EXPR_IDENTIFIER;

    MALLOC(expression->identifier, sizeof(struct Identifier));
    MALLOC(expression->identifier->name,
           strlen((const char *)token->lexeme) + 1);

    memcpy(expression->identifier->name,
           token->lexeme,
           strlen((const char *)token->lexeme) + 1);

    return expression;
}

struct Expression *create_number(struct Token *token) {
    struct Expression *expression;
    MALLOC(expression, sizeof(struct Expression));

    expression->kind = EXPR_LITERAL;

    MALLOC(expression->literal, sizeof(struct Literal));
    expression->literal->kind = LIT_NUMBER;
    expression->literal->number = atoi((char *)token->lexeme);

    return expression;
}

struct Expression *create_procedure_call(void) {
    struct ProcedureCall *procedure_call;
    MALLOC(procedure_call, sizeof(struct ProcedureCall));
    MALLOC(procedure_call->operand_list, sizeof(struct OperandList));

    procedure_call->operator_ = NULL;
    STAILQ_INIT(procedure_call->operand_list);

    struct Expression *expression;
    MALLOC(expression, sizeof(struct Expression));

    expression->kind = EXPR_PROCEDURE_CALL;
    expression->procedure_call = procedure_call;

    return expression;
}

void update_procedure_call(struct Expression *expression,
                           struct ProcedureCall *procedure_call) {
    if (procedure_call->operator_ == NULL) {
        MALLOC(procedure_call->operator_, sizeof(struct Expression));
        procedure_call->operator_ = expression;
        return;
    }

    struct Operand *operand;
    MALLOC(operand, sizeof(struct Operand *));
    operand->expression = expression;

    STAILQ_INSERT_TAIL(procedure_call->operand_list, operand, entries);
}

void update_expression(struct Expression *src_expression,
                       struct Expression *dest_expression) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
    switch (dest_expression->kind) {
#pragma clang diagnostic pop
        case EXPR_PROCEDURE_CALL:
            update_procedure_call(src_expression,
                                  dest_expression->procedure_call);
            break;
        default:
            fprintf(
                stderr, "Cannot update expression: %d", dest_expression->kind);
            exit(1);
    }
}

inline void push_expression(struct Expression *expression,
                            struct ContextStack *context_stack) {
    struct Context *context;
    MALLOC(context, sizeof(struct Context));

    context->expression = expression;

    SLIST_INSERT_HEAD(context_stack, context, entries);
}

inline struct Expression *pop_expression(struct ContextStack *context_stack) {
    struct Context *context = SLIST_FIRST(context_stack);
    SLIST_REMOVE_HEAD(context_stack, entries);
    return context->expression;
}

struct ContextStack *init_context_stack(void) {
    struct ContextStack *context_stack;
    MALLOC(context_stack, sizeof(struct ContextStack));
    SLIST_INIT(context_stack);
    return context_stack;
}

void free_context_stack(struct ContextStack *context_stack) {
    struct Context *context, *context_;

    SLIST_FOREACH_SAFE(context, context_stack, entries, context_) {
        SLIST_REMOVE(context_stack, context, Context, entries);
        free(context);
    }

    free(context_stack);
}

struct Expression *parse(struct TokenList *token_list) {
    struct ContextStack *context_stack = init_context_stack();
    struct Expression *expression = NULL;
    struct Token *token;

    STAILQ_FOREACH(token, token_list, entries) {
        bool will_update = true;

        switch (token->kind) {
            case TOKEN_IDENTIFIER:
                expression = create_identifier(token);
                break;
            case TOKEN_NUMBER:
                expression = create_number(token);
                break;
            case TOKEN_PAREN_L:
                expression = create_procedure_call();
                push_expression(expression, context_stack);
                will_update = false;
                break;
            case TOKEN_PAREN_R:
                expression = pop_expression(context_stack);
                break;
        }

        will_update &= !SLIST_EMPTY(context_stack);

        if (will_update) {
            struct Context *top_context = SLIST_FIRST(context_stack);
            update_expression(expression, top_context->expression);
        }
    }

    free_context_stack(context_stack);

    return expression;
}
