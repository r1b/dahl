#ifndef VM_H
#define VM_H

#include "compiler.h"

struct VmStackValue
{
    union RuntimeValue value;
    struct VmStackValue *next;
    struct VmStackValue *prev;
};

struct VmStack
{
    struct VmStackValue *top;
    struct VmStackValue *bottom;
};

union RuntimeValue *vm_exec(struct InstructionList *);
union RuntimeValue *vm_stack_pop(void);
void vm_stack_push(union RuntimeValue *);

#endif
