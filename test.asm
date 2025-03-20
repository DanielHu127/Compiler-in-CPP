    bits  64
    default rel

    section .data
        msg db 'Hello, world!', 0xa
        len equ $ - msg
    ; State of the stack before jumping to ExitProcess.
    struc stk
        resq  4   ; shadow area.
        ;resq  1   ; to realign RSP to DQWORD.
    endstruc

    section .text
    extern GetStdHandle
    extern WriteConsoleA
    extern ExitProcess

    global _start
_start:
    sub rsp,stk_size

    mov ecx, -11
    call GetStdHandle
    mov rbx, rax

    lea rdx, [msg]
    mov rcx, rbx
    mov r8, 13
    lea r9, [rsp+32]
    sub rsp, 32
    call WriteConsoleA
    add rsp, 32

    ; ExitProcess takes just one argument in ECX.
    ; And we can jump (instead of call) because ExitProcess don't return.
    xor   ecx,ecx
    call   ExitProcess
