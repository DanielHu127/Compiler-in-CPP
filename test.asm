default rel   ; Use RIP-relative addressing by default
; Data section
section .data
    msg db "Hello, World!", 13, 10  ; Message with CR+LF
    msglen equ $ - msg              ; Length of message
    STD_OUTPUT_HANDLE equ -11       ; Windows constant for stdout
    NULL equ 0
    
section .bss
    stdout resq 1                   ; Reserve space for handle
    written resq 1                  ; Reserve space for bytes written
; Import necessary Windows functions
extern GetStdHandle
extern WriteConsoleA
extern ExitProcess
; Code section
section .text
    global main
main:
    ; Set up stack frame
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32                ; Shadow space for function calls
    ; Get handle to stdout
    mov     ecx, STD_OUTPUT_HANDLE
    call    GetStdHandle
    mov     qword [rel stdout], rax
    ; Write to console
    mov     rcx, qword [rel stdout] ; Console handle
    lea     rdx, [rel msg]         ; Message to write
    mov     r8, msglen             ; Message length
    lea     r9, [rel written]      ; Bytes written
    push    NULL                   ; Overlapped
    sub     rsp, 32                ; Shadow space
    call    WriteConsoleA
    add     rsp, 40                ; Clean up stack (32 + 8)
    ; Exit program
    xor     ecx, ecx               ; Exit code 0
    call    ExitProcess
    ; Function epilogue
    leave
    ret