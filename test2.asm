bits 64
default rel
section .data
    message db "Hello, Console!", 0   ; Null-terminated string

section .text
    global _start
    extern GetStdHandle, WriteConsoleA, ExitProcess

_start:
    ; Get the handle to standard output (stdout)
    and rsp, -16
    sub rsp, 32                     ; Allocate shadow space
    mov ecx, -11                    ; STD_OUTPUT_HANDLE
    call GetStdHandle
    mov rbx, rax                    ; Save stdout handle in rbx

    ; Write message to console
    lea rdx, [message]              ; Address of the string
    mov rcx, rbx                    ; Console handle
    mov r8, 15                      ; String length
    lea r9, [rsp+32]                ; Pointer to DWORD to store number of characters written
    sub rsp, 32                     ; Shadow space
    call WriteConsoleA
    add rsp, 32                     ; Restore shadow space

    ; Exit the process
    mov ecx, -1                    ; Exit code = 0
    call ExitProcess