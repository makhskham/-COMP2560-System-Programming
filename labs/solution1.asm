INCLUDE Irvine32.inc
INCLUDELIB Irvine32.lib

INCLUDELIB kernel32.lib
INCLUDELIB user32.lib

.data
array DWORD 5, 10, 15, 20, 25

.code
main PROC
    ; Direct addressing to add first two elements
    mov eax, array       ; Load first element (5)
    add eax, array + 4   ; Add second element (10) - note the +4 for DWORD size
    
    ; Indirect addressing to subtract third element
    lea esi, array       ; Load base address of array
    add esi, 8          ; Point to third element (offset 8 = 2 * DWORD)
    sub eax, [esi]      ; Subtract third element (15)
    
    ; Store result in fifth element
    mov array + 16, eax  ; Store in fifth element (offset 16 = 4 * DWORD)
    
    ; Display result
    ; Don't clear EAX here - remove the xor eax, eax
    call WriteInt        ; Display the result
    call Crlf           ; New line for formatting
    
    exit
main ENDP
END main