#include "parser.h"
#include "lexer.h"
#include "utils.h"

inline bool is_lambda(struct Token *token) {
    return (token != NULL && token->kind == TOKEN_IDENTIFIER &&
            strcmp((const char *)token->lexeme, "lambda") == 0);
}

inline struct Expression *create_expression(enum ExpressionKind kind) {
    struct Expression *expression;
    MALLOC(expression, sizeof(struct Expression));

    expression->kind = kind;
    expression->parent = NULL;

    return expression;
}

struct Expression *create_identifier(struct Token *token) {
    struct Expression *expression = create_expression(EXPR_IDENTIFIER);

    MALLOC(expression->identifier, sizeof(struct Identifier));
    MALLOC(expression->identifier->name,
           strlen((const char *)token->lexeme) + 1);

    memcpy(expression->identifier->name,
           token->lexeme,
           strlen((const char *)token->lexeme) + 1);

    return expression;
}

struct Expression *create_number(struct Token *token) {
    struct Expression *expression = create_expression(EXPR_LITERAL);

    MALLOC(expression->literal, sizeof(struct Literal));
    expression->literal->kind = LIT_NUMBER;
    expression->literal->number = atoi((char *)token->lexeme);

    return expression;
}

struct Expression *create_lambda(struct TokenList **token_list) {
    struct Expression *expression = create_expression(EXPR_LAMBDA);

    struct Lambda *lambda;
    MALLOC(lambda, sizeof(struct Lambda));
    MALLOC(lambda->operand_list, sizeof(struct OperandList));
    MALLOC(lambda->body, sizeof(struct Expression));

    STAILQ_INIT(lambda->operand_list);
    lambda->body = NULL;

    struct Token *token;
    bool done = false;

    while (!done && !STAILQ_EMPTY(*token_list)) {
        token = STAILQ_FIRST(*token_list);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
        switch (token->kind) {
#pragma clang diagnostic pop
            case TOKEN_PAREN_R:
                done = true;
                break;
            case TOKEN_IDENTIFIER: {
                struct Operand *operand;
                MALLOC(operand, sizeof(struct Operand *));
                operand->expression = create_identifier(token);
                STAILQ_INSERT_TAIL(lambda->operand_list, operand, entries);
            } break;
            default:
                fprintf(stderr,
                        "Invalid lambda operand %d: %s",
                        token->kind,
                        token->lexeme);
                exit(1);
        }
        STAILQ_REMOVE_HEAD(*token_list, entries);
    }

    expression->lambda = lambda;

    return expression;
}

struct Expression *create_procedure_call(void) {
    struct Expression *expression = create_expression(EXPR_PROCEDURE_CALL);

    struct ProcedureCall *procedure_call;
    MALLOC(procedure_call, sizeof(struct ProcedureCall));
    MALLOC(procedure_call->operand_list, sizeof(struct OperandList));

    procedure_call->operator_ = NULL;
    STAILQ_INIT(procedure_call->operand_list);

    expression->procedure_call = procedure_call;

    return expression;
}

inline void set_operator(struct ProcedureCall *procedure_call,
                         struct Expression *expression) {
    MALLOC(procedure_call->operator_, sizeof(struct Expression));
    procedure_call->operator_ = expression;
}

inline void insert_operand(struct ProcedureCall *procedure_call,
                           struct Expression *expression) {
    struct Operand *operand;
    MALLOC(operand, sizeof(struct Operand *));
    operand->expression = expression;

    STAILQ_INSERT_TAIL(procedure_call->operand_list, operand, entries);
}

void update_expression(struct Expression *parent, struct Expression *child) {
    child->parent = parent;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
    switch (parent->kind) {
#pragma clang diagnostic pop
        case EXPR_LAMBDA:
            parent->lambda->body = child;
            break;
        case EXPR_PROCEDURE_CALL:
            parent->procedure_call->operator_ == NULL
                ? set_operator(parent->procedure_call, child)
                : insert_operand(parent->procedure_call, child);
            break;
        default:
            fprintf(stderr, "Cannot update expression: %d", parent->kind);
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

    while (!STAILQ_EMPTY(token_list)) {
        bool will_update = true;
        token = STAILQ_FIRST(token_list);

        switch (token->kind) {
            case TOKEN_IDENTIFIER:
                expression = create_identifier(token);
                break;
            case TOKEN_NUMBER:
                expression = create_number(token);
                break;
            case TOKEN_PAREN_L: {
                if (is_lambda(STAILQ_NEXT(token, entries))) {
                    expression = create_lambda(&token_list);
                } else {
                    expression = create_procedure_call();
                }
                will_update = false;
                push_expression(expression, context_stack);
            } break;
            case TOKEN_PAREN_R:
                expression = pop_expression(context_stack);
                // TODO: Validate the expression
                break;
        }

        will_update &= !SLIST_EMPTY(context_stack);

        if (will_update) {
            struct Context *top_context = SLIST_FIRST(context_stack);
            update_expression(top_context->expression, expression);
        }

        STAILQ_REMOVE_HEAD(token_list, entries);
    }

    free_context_stack(context_stack);

    return expression;
}
