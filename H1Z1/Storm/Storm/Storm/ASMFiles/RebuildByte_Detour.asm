EXTERNDEF Fixup_ReturnOriginal: QWORD

.code

RebuildByte_Detour proc

   	;Jmp To Rebuild Stolen Bytes
	;INT 3
    ;JMP [Fixup_ReturnOriginal]

RebuildByte_Detour endp

end