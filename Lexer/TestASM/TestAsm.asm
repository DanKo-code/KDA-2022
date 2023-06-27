.586
.MODEL FLAT, STDCALL
includelib libucrt.lib
includelib kernel32.lib
includelib "C:\2курс, 1 сем\КПО\20\Lexer_2\Lexer\Debug\StaticLibrary.lib"
ExitProcess PROTO :DWORD

casual PROTO : DWORD
printUint PROTO : DWORD
printString PROTO : DWORD
shiftChar PROTO : DWORD, : DWORD, : DWORD
stringCopy PROTO : DWORD, : DWORD
strconcat PROTO : DWORD, :DWORD
STRLEN PROTO : DWORD
BREAKL PROTO


.STACK 4096


.CONST


.data	
storona BYTE 1,'['
symbol BYTE 1,'a'
ns DWORD 10

BUFFER BYTE 0

.code

main PROC

PUSH OFFSET symbol
PUSH ns

POP EDX
POP EBX

INVOKE shiftChar,OFFSET storona, EBX, EDX

MOV BUFFER, EAX

PUSH 0
CALL ExitProcess
main ENDP

END MAIN