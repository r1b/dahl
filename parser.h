#include "lexer.h"

// FIXME: Naming conflicts with lex
enum ExpressionKind {IDENT, LITERAL, PROCEDURE_CALL};
enum LiteralKind {NUM};

struct Identifier {
    char* name;
};

struct Literal {
    enum LiteralKind kind;
    union {
        int number;
    };
};

struct Operand {
    struct Expression* value;
    struct Operand* next;
    struct Operand* prev;
};

struct OperandList {
    struct Operand* head;
    struct Operand* tail;
};

struct ProcedureCall {
    struct Expression* operator;
    struct OperandList* operand_list;
};

struct Expression {
    enum ExpressionKind kind;
    union {
        struct Identifier* identifier;
        struct Literal* literal;
        struct ProcedureCall* procedure_call;
    };
};

struct Context {
    struct Expression* expression;
    struct Context* prev;
    struct Context* next;
};

struct ContextStack {
    struct Context* top;
    struct Context* bottom;
};

void init_context_stack(struct ContextStack*);
void init_context(struct Context**);
void push_context(struct Context*, struct ContextStack*);
struct Context* pop_context(struct ContextStack*);

void update_procedure_call(struct Expression*, struct ProcedureCall*);
void update_expression(struct Expression*, struct Expression*);

struct Expression* parse(struct TokenList*);
struct Expression* parse_expression(struct Token*, struct ContextStack*);
void display_expression(struct Expression* expression);
