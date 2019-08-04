// TODO: Eventually this will actually be bytecode and not nasty structs.
// In fact, everything so far should have a more optimized representation.
// And we should not malloc when executing instructions right
// I have no idea what i'm doing
// help

#include "vm.h"
#include "utils.h"

union RuntimeValue *vm_exec_add(struct VmStack *vm_stack) {
    union RuntimeValue *x, *y, *result;

    MALLOC(result, sizeof(union RuntimeValue));

    x = vm_exec_pop(vm_stack);
    y = vm_exec_pop(vm_stack);

    result->immediate = x->immediate + y->immediate;

    return result;
}

union RuntimeValue *vm_exec_mul(struct VmStack *vm_stack) {
    union RuntimeValue *x, *y, *result;

    MALLOC(result, sizeof(union RuntimeValue));

    x = vm_exec_pop(vm_stack);
    y = vm_exec_pop(vm_stack);

    result->immediate = x->immediate * y->immediate;

    return result;
}

union RuntimeValue *vm_exec_pop(struct VmStack *vm_stack) {
    struct VmStackValue *vm_stack_value = SLIST_FIRST(vm_stack);
    SLIST_REMOVE_HEAD(vm_stack, entries);
    return vm_stack_value->value;
}

void vm_exec_push(union RuntimeValue *runtime_value, struct VmStack *vm_stack) {
    struct VmStackValue *vm_stack_value;
    MALLOC(vm_stack_value, sizeof(struct VmStackValue));
    vm_stack_value->value = runtime_value;
    SLIST_INSERT_HEAD(vm_stack, vm_stack_value, entries);
}

union RuntimeValue *vm_exec(struct InstructionList *instruction_list) {
    struct VmStack *vm_stack;
    MALLOC(vm_stack, sizeof(struct VmStack));
    SLIST_INIT(vm_stack);

    struct Instruction *instruction;

    union RuntimeValue *runtime_value;
    MALLOC(runtime_value, sizeof(union RuntimeValue));

    STAILQ_FOREACH(instruction, instruction_list, entries) {
        switch (instruction->kind) {
            case INSTRUCTION_ADD:
                vm_exec_push(vm_exec_add(vm_stack), vm_stack);
                break;
            case INSTRUCTION_MUL:
                vm_exec_push(vm_exec_mul(vm_stack), vm_stack);
                break;
            case INSTRUCTION_PUSH:
                vm_exec_push(instruction->operand, vm_stack);
                break;
            case INSTRUCTION_POP:
                runtime_value = vm_exec_pop(vm_stack);
                break;
        }
    }

    return runtime_value;
}
