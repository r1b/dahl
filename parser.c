#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "utils.h"

struct Context *init_context(void)
{
    struct Context *context;
    MALLOC(context, sizeof(struct Context));
    MALLOC(context->expression, sizeof(struct Expression));
    return context;
}

struct ProcedureCall *init_procedure_call(void)
{
    struct ProcedureCall *procedure_call;
    MALLOC(procedure_call, sizeof(struct ProcedureCall));
    MALLOC(procedure_call->operand_list, sizeof(struct OperandList));

    procedure_call->operator_ = NULL;
    TAILQ_INIT(procedure_call->operand_list);

    return procedure_call;
}

void update_procedure_call(struct Expression *expression, struct ProcedureCall *procedure_call)
{
    if (procedure_call->operator_ == NULL)
    {
        MALLOC(procedure_call->operator_, sizeof(struct Expression));
        procedure_call->operator_ = expression;
        return;
    }

    struct Operand *operand;
    MALLOC(operand, sizeof(struct Operand *));
    operand->expression = expression;

    TAILQ_INSERT_TAIL(procedure_call->operand_list, operand, entries);
}

void update_expression(struct Expression *src_expression, struct Expression *dest_expression)
{
    switch (dest_expression->kind)
    {
    case EXPR_PROCEDURE_CALL:
        update_procedure_call(src_expression, dest_expression->procedure_call);
        break;
    default:
        fprintf(stderr, "Cannot update expression: %d", dest_expression->kind);
        exit(1);
    }
}

struct Expression *parse(struct TokenList *token_list)
{
    struct ContextStack *context_stack;
    struct Expression *expression;
    struct Token *token;

    MALLOC(context_stack, sizeof(struct ContextStack));
    SLIST_INIT(context_stack);

    expression = NULL;

    STAILQ_FOREACH(token, token_list, entries)
    {
        // FIXME: Don't do this dumb shit just pop the last one
        expression = parse_expression(token, context_stack);
    }

    free(context_stack);

    return expression;
}

struct Expression *parse_expression(struct Token *token, struct ContextStack *context_stack)
{
    struct Context *context;
    struct Expression *expression;

    MALLOC(expression, sizeof(struct Expression));

    switch (token->kind)
    {
    case TOKEN_IDENTIFIER:
        expression->kind = EXPR_IDENTIFIER;
        MALLOC(expression->identifier, sizeof(struct Identifier));
        MALLOC(expression->identifier->name, strlen((const char *)token->lexeme) + 1);
        memcpy(expression->identifier->name, token->lexeme, strlen((const char *)token->lexeme) + 1);
        break;
    case TOKEN_NUMBER:
        expression->kind = EXPR_LITERAL;
        MALLOC(expression->literal, sizeof(struct Literal));
        expression->literal->kind = LIT_NUMBER;
        // FIXME: This won't work for numeric tower
        expression->literal->number = atoi((char *)token->lexeme);
        break;
    case TOKEN_PAREN_L:
        MALLOC(expression->procedure_call, sizeof(struct ProcedureCall));

        expression->kind = EXPR_PROCEDURE_CALL;
        expression->procedure_call = init_procedure_call();

        context = init_context();
        context->expression = expression;

        SLIST_INSERT_HEAD(context_stack, context, entries);
        return expression;
    case TOKEN_PAREN_R:
        context = SLIST_FIRST(context_stack);
        SLIST_REMOVE_HEAD(context_stack, entries);
        expression = context->expression;
        break;
    }

    if (!SLIST_EMPTY(context_stack))
    {
        update_expression(expression, (SLIST_FIRST(context_stack))->expression);
    }

    return expression;
}

void display_expression(struct Expression *expression)
{
    fprintf(stdout, "TODO");
}
