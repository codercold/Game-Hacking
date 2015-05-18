PrologueMsg PROTO

EXTERNDEF PDevice_Present: QWORD
EXTERNDEF Fixup_Steam_Jump_Bytes: QWORD

.CODE

Prolog_Present proc
	
	;MOV RAX, RCX
	;INT 3
	MOV PDevice_Present, RCX		
	MOV RAX, [RCX]

	; Save Registers
	PUSH RAX
	PUSH RCX
	PUSH RDX
	;PUSH RBX
	;PUSH RSP
	;PUSH RBP
	PUSH RSI
	PUSH RDI
	
	PUSH R8
	PUSH R9
	PUSH R10
	PUSH R11
	PUSH R12
	PUSH R13
	PUSH R14
	PUSH R15

	CALL PrologueMsg

	; Replace Registers
	POP R15
	POP R14
	POP R13
	POP R12
	POP R11
	POP R10
	POP R9
	POP R8

	POP RDI
	POP RSI
	;POP RBP
	;POP RSP
	;POP RBX
	POP RDX
	POP RCX
	POP RAX
	
	; Jmp To Gameoverlay Hook
	;INT 3
	JMP [Fixup_Steam_Jump_Bytes]	

Prolog_Present endp

end