#ifndef VM_H
#define VM_H

#include "compiler.h"
#include <sys/queue.h>

struct VmStackValue {
    union RuntimeValue *value;
    SLIST_ENTRY(VmStackValue) entries;
};

SLIST_HEAD(VmStack, VmStackValue);

union RuntimeValue *vm_exec(struct InstructionList *);
union RuntimeValue *vm_exec_add(struct VmStack *);
union RuntimeValue *vm_exec_mul(struct VmStack *);
union RuntimeValue *vm_exec_pop(struct VmStack *);
void vm_exec_push(union RuntimeValue *, struct VmStack *);

#endif
