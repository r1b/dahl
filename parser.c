#include "lexer.h"
#include "parser.h"

struct Expression* parse(struct TokenList* token_list) {
    struct Parser* parser;
    struct Expression* expression;

    MALLOC(parser, sizeof(struct Parser));
    MALLOC(parser->context_stack, sizeof(struct ContextStack));

    MALLOC(expression, sizeof(struct Expression));

    parser->token = token_list->head;
    init_context_stack(parser->context_stack);

    while(parser->token != NULL) {
        expression = parse_expression(parser);
        parser->token = parser->token->next;
    }

    return expression;
}

Expression* parse_expression(struct Parser* parser) {
    struct Context* context;
    struct Expression expression;

    MALLOC(expression, sizeof(struct Expression));

    switch (parser->token->kind) {
        case IDENTIFIER:
            expression->kind = IDENT;
            MALLOC(expression->identifier, sizeof(struct Identifier);
            expression->identifier->name = cur->value;
            break;
        case NUMBER:
            expression->kind = LITERAL;
            MALLOC(expression->literal, sizeof(struct Literal);
            expression->literal->kind = NUM;
            expression->literal->value = cur->value;
            break;
        case PAREN_L:
            MALLOC(context, sizeof(struct Context));
            MALLOC(context->expression, sizeof(struct Expression));
            MALLOC(expression->procedure_call, sizeof(struct ProcedureCall);

            expression->kind = PROCEDURE_CALL;
            context->expression = expression;

            context_push(context, parser->context_stack)
            break;
        case PAREN_R:
            MALLOC(context, sizeof(struct Context));
            MALLOC(context->expression, sizeof(struct Expression));
            context = context_pop(parser->context-stack);
            expression = context->expression;
            break;
        default:
            fprintf(stderr, "Unrecognized token kind: %d", (int)cur->kind);
            exit(1);
            break;
    }

    if (parser->context_stack->top != NULL) {
        update_top_context(expression, parser->context_stack);
    }

    return expression;
}
