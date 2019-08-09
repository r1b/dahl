#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"
#include <sys/queue.h>

union RuntimeValue {
    void *address;
    int immediate;
};

enum InstructionKind {
    INSTRUCTION_ADD,
    // INSTRUCTION_CALL,
    INSTRUCTION_MUL,
    INSTRUCTION_POP,
    INSTRUCTION_PUSH,
};

struct Instruction {
    enum InstructionKind kind;
    union RuntimeValue *operand;
    STAILQ_ENTRY(Instruction) entries;
};

STAILQ_HEAD(InstructionList, Instruction);

struct InstructionList *compile(struct Expression *);
void compile_expression(struct ContextStack *, struct InstructionList *);
void push_procedure_call_context(struct ProcedureCall *, struct ContextStack *);

#endif
