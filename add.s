section .data
    num1 dw 5                  ; First number
    num2 dw 10                 ; Second number
    result dw 0                ; Variable to store the result
    msg db 'Result: ', 0       ; Message to display

section .text
    extern GetStdHandle
    extern WriteFile
    extern ExitProcess
    global _start

_start:
    ; Calculate result
    MOV AX, [num1]             ; Load num1 into AX
    ADD AX, [num2]             ; Add num2 to AX
    MOV [result], AX           ; Store result in memory

    ; Prepare to display the result
    ; (Converting the number to a string is needed here for display)
    ; This example skips the conversion for simplicity.

    ; Exit
    xor rdi, rdi               ; Exit code 0
    call ExitProcess           ; Call exit process
