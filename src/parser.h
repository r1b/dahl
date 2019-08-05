#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <sys/queue.h>

enum ExpressionKind { EXPR_IDENTIFIER, EXPR_LITERAL, EXPR_PROCEDURE_CALL };
enum LiteralKind { LIT_NUMBER };

struct Identifier {
    char *name;
};

struct Literal {
    enum LiteralKind kind;
    union {
        int number;
    };
};

struct Operand {
    struct Expression *expression;
    STAILQ_ENTRY(Operand) entries;
};

STAILQ_HEAD(OperandList, Operand);

struct ProcedureCall {
    struct Expression *operator_; // FIXME: naming conflict
    struct OperandList *operand_list;
};

struct Expression {
    enum ExpressionKind kind;
    union {
        struct Identifier *identifier;
        struct Literal *literal;
        struct ProcedureCall *procedure_call;
    };
};

struct Context {
    struct Expression *expression;
    SLIST_ENTRY(Context) entries;
};

SLIST_HEAD(ContextStack, Context);

struct Context *init_context(void);

struct ProcedureCall *init_procedure_call(void);
void update_procedure_call(struct Expression *, struct ProcedureCall *);
void update_expression(struct Expression *, struct Expression *);

struct Expression *parse(struct TokenList *);
struct Expression *parse_expression(struct Token *, struct ContextStack *);

#endif
