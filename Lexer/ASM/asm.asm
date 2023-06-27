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

print_error_les_zero PROTO
checkstringconcat PROTO : DWORD, :DWORD
print_error_str_literal_length_exceeded PROTO

concatStrings PROTO : DWORD, : DWORD, : DWORD
getHash PROTO : DWORD
printSymbols PROTO : DWORD, : DWORD

.STACK 4096

.CONST
L_0 BYTE 11, "turned out "
L_1 BYTE 5, "next "
L_2 BYTE 6, "hash: "
L_3 DWORD 1
L_4 BYTE 1,'R'
L_5 DWORD 100
L_6 BYTE 5, "first"
L_7 BYTE 6, "second"
L_8 BYTE 5, "third"
L_9 DWORD 100


;Константы для предачи в shiftChar
charLeftShift BYTE 1,'['
charRightShift BYTE 1,']'

.data

concatStrings@a BYTE 252 DUP(0)
concatStrings@b BYTE 252 DUP(0)
concatStrings@c BYTE 252 DUP(0)
getHash@numb DWORD 0
getHash@randomCount DWORD 0
getHash@hash DWORD 0
getHash@res BYTE 252 DUP(0)
printSymbols@symbol BYTE 252 DUP(0)
printSymbols@shiftsNumb DWORD 0
main@symbol BYTE 252 DUP(0)
main@first BYTE 252 DUP(0)
main@second BYTE 252 DUP(0)
main@third BYTE 252 DUP(0)
main@concatResalt BYTE 252 DUP(0)
main@randomNumb DWORD 0

;Чтобы не сбивался счетчик, когда мы заходим в функцию SL
RemECX DWORD 0

.code
main PROC

;---обработка---выражения---после---=
PUSH OFFSET L_4
PUSH OFFSET main@symbol
CALL stringCopy
;---вызов---void---функции---
PUSH OFFSET main@symbol
PUSH L_5
CALL printSymbols
;---обработка---выражения---после---=
PUSH OFFSET L_6
PUSH OFFSET main@first
CALL stringCopy
;---обработка---выражения---после---=
PUSH OFFSET L_7
PUSH OFFSET main@second
CALL stringCopy
;---обработка---выражения---после---=
PUSH OFFSET L_8
PUSH OFFSET main@third
CALL stringCopy
;---обработка---выражения---после---=
PUSH OFFSET main@first
PUSH OFFSET main@second
PUSH OFFSET main@third
CALL concatStrings
PUSH EAX
PUSH OFFSET main@concatResalt
CALL stringCopy
;---Распечатка---
PUSH OFFSET main@concatResalt
CALL printString
;---обработка---выражения---после---=
PUSH L_9
CALL getHash
PUSH EAX
POP EAX
MOV main@randomNumb, EAX


INVOKE ExitProcess, 0
error_les_zero:
CALL print_error_les_zero
INVOKE ExitProcess, 1
error_str_literal_length_exceeded:
CALL print_error_str_literal_length_exceeded
INVOKE ExitProcess, 2
main ENDP


;-----------FUNCTION concatStrings-----------

concatStrings PROC uses EBX ECX EDX, concatStrings@c_STRING : DWORD, concatStrings@b_STRING : DWORD, concatStrings@a_STRING : DWORD
PUSH concatStrings@a_STRING
PUSH OFFSET concatStrings@a
CALL stringCopy
PUSH concatStrings@b_STRING
PUSH OFFSET concatStrings@b
CALL stringCopy
PUSH concatStrings@c_STRING
PUSH OFFSET concatStrings@c
CALL stringCopy
PUSH OFFSET concatStrings@a
PUSH OFFSET concatStrings@b
POP EDX
POP EBX
INVOKE strconcat,EBX, EDX
PUSH EAX
PUSH OFFSET concatStrings@c
POP EDX
POP EBX
INVOKE strconcat,EBX, EDX
PUSH EAX
POP EAX
RET
concatStrings ENDP
;-----------FUNCTION getHash-----------

getHash PROC uses EBX ECX EDX, getHash@numb_UINT : DWORD
MOV EAX, getHash@numb_UINT
MOV getHash@numb, EAX
;---обработка---выражения---после---=
PUSH getHash@numb
CALL casual
PUSH EAX
POP EAX
MOV getHash@randomCount, EAX
MOV ECX, getHash@randomCount
CMP ECX,0
JE skipcycle
	CYCLE:
MOV RemECX, ECX
;---обработка---выражения---после---=
PUSH getHash@hash
PUSH getHash@numb
CALL casual
PUSH EAX
POP EDX
POP EBX
ADD EBX,EDX
PUSH EBX
POP EAX
MOV getHash@hash, EAX
MOV ECX, RemECX
	loop CYCLE
skipcycle:
;---обработка---выражения---после---=
PUSH OFFSET L_0
PUSH OFFSET L_1
PUSH OFFSET L_2
CALL concatStrings
PUSH EAX
PUSH OFFSET getHash@res
CALL stringCopy
;---Распечатка---
PUSH OFFSET getHash@res
CALL printString
CALL BREAKL
;---Распечатка---
PUSH getHash@hash
CALL printUint
CALL BREAKL
PUSH getHash@hash
POP EAX
RET
getHash ENDP
;-----------FUNCTION printSymbols-----------

printSymbols PROC uses EBX ECX EDX, printSymbols@shiftsNumb_UINT : DWORD, printSymbols@symbol_CHAR : DWORD
PUSH printSymbols@symbol_CHAR
PUSH OFFSET printSymbols@symbol
CALL stringCopy
MOV EAX, printSymbols@shiftsNumb_UINT
MOV printSymbols@shiftsNumb, EAX
MOV ECX, printSymbols@shiftsNumb
CMP ECX,0
JE skipcycle
	CYCLE:
MOV RemECX, ECX
;---обработка---выражения---после---=
PUSH OFFSET printSymbols@symbol
PUSH L_3
POP EDX
POP EBX
INVOKE shiftChar,OFFSET charLeftShift, EBX, EDX
PUSH EAX
PUSH OFFSET printSymbols@symbol
CALL stringCopy
;---Распечатка---
PUSH OFFSET printSymbols@symbol
CALL printString
CALL BREAKL
MOV ECX, RemECX
	loop CYCLE
skipcycle:
RET
printSymbols ENDP
END MAIN
