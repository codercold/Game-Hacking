PrologueMsg PROTO

EXTERNDEF PDevice_Present: QWORD
EXTERNDEF Fixup_Steam_Jump_Bytes: QWORD

.CODE

Prolog_Present proc
	
	MOV RAX, RCX
	MOV PDevice_Present, RAX		
	CALL PrologueMsg

	; Jmp To Gameoverlay Hook
	;INT 3
	JMP [Fixup_Steam_Jump_Bytes]	

Prolog_Present endp

end