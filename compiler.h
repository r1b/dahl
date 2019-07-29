#ifndef COMPILER_H
#define COMPILER_H

#include <sys/queue.h>
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
    // clang-format off
    STAILQ_ENTRY(Instruction) entries;
    // clang-format on
};

STAILQ_HEAD(InstructionList, Instruction);

struct InstructionList *compile(struct Expression *);
void *compile_expression(struct ContextStack *, struct InstructionList *);
void push_procedure_call_context(struct ProcedureCall *, struct ContextStack *);

#endif
