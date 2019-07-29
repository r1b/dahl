#ifndef VM_H
#define VM_H

#include <sys/queue.h>
#include "compiler.h"

struct VmStackValue
{
    union RuntimeValue *value;
    // clang-format off
    SLIST_ENTRY(VmStackValue) entries;
    // clang-format on
};

SLIST_HEAD(VmStack, VmStackValue);

// FIXME: These names make me want to puke emoji
// I think I'm just in a bad mood so will re-evaluate later

union RuntimeValue *vm_exec(struct InstructionList *);
union RuntimeValue *vm_exec_add(struct VmStack *);
union RuntimeValue *vm_exec_mul(struct VmStack *);
union RuntimeValue *vm_exec_pop(struct VmStack *);
void vm_exec_push(union RuntimeValue *, struct VmStack *);

#endif
