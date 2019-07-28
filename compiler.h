#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"

enum InstructionKind
{
    INSTRUCTION_ADD,
    // INSTRUCTION_CALL,
    INSTRUCTION_MUL,
    INSTRUCTION_POP,
    INSTRUCTION_PUSH,
};

union RuntimeValue {
    void *address;
    int immediate;
};

struct Instruction
{
    enum InstructionKind kind;
    union RuntimeValue *operand;
    struct Instruction *next;
    struct Instruction *prev;
};

struct InstructionList
{
    struct Instruction *head;
    struct Instruction *tail;
};

struct InstructionList *compile(struct Expression *);
void *compile_expression(struct ContextStack *, struct InstructionList *);
void push_procedure_call_context(struct ProcedureCall *, struct ContextStack *);

struct InstructionList *add_instruction(struct Instruction *, struct InstructionList *);
struct InstructionList *init_instruction_list(void);

#endif