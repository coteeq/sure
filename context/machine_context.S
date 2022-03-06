#if (APPLE)
  #define FUNCTION_NAME(name) _##name
#else
  #define FUNCTION_NAME(name) name
#endif

.global FUNCTION_NAME(SetupMachineContext)
.global FUNCTION_NAME(SwitchMachineContext)

# https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86/

# SetupMachineContext(rsp, trampoline, arg)

FUNCTION_NAME(SetupMachineContext):
    # Arguments
    # 1) %rdi - bottom of the stack
    # 2) %rsi - MachineContextTrampoline address
    # 3) %rdx - argument (user trampoline)

    # Switch to stack
    movq %rsp, %rbx # tmp := rsp
    movq %rdi, %rsp # rsp := rdi

    # Just to be safe
    subq $7, %rsp

    # %rsp must be 16-byte aligned before MachineContextTrampoline call
    andq $-16, %rsp

    # 7-th argument for MachineContextTrampoline
    movq %rdx, 8(%rsp)

    # Prepare for context switch

    # 1) Return address for SwitchMachineContext
    pushq %rsi

    # 2) Callee-saved registers
    pushq $0 # r15
    pushq $0 # r14
    pushq $0 # r13
    pushq $0 # r12
    pushq $0 # rbx
    pushq $0 # rbp

    # Return value for SetupMachineContext
    movq %rsp, %rax

    # Restore callstack
    movq %rbx, %rsp # rsp := tmp

    retq


# SwitchMachineContext(from_rsp, to_rsp)

FUNCTION_NAME(SwitchMachineContext):
    # SwitchMachineContext frame created on top of the current stack

    # 1. Save current execution context to 'from'

    # 1.1 Save callee-saved registers on top of the current stack

    # https://stackoverflow.com/questions/18024672/what-registers-are-preserved-through-a-linux-x86-64-function-call
    # https://uclibc.org/docs/psABI-x86_64.pdf

    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12

    pushq %rbx
    pushq %rbp

    # Switch stacks

    # 1.2 Save current stack pointer to 'from' MachineContext
    movq %rsp, (%rdi) # from_rsp := rsp

    # 2. Activate 'to' machine context

    # 2.1 Set stack pointer to target stack
    movq (%rsi), %rsp # rsp := to_rsp

    # 2.2 Restore and pop registers saved on target stack

    popq %rbp
    popq %rbx

    popq %r12
    popq %r13
    popq %r14
    popq %r15

    # Pop current SwitchMachineContext frame from target stack

    retq
