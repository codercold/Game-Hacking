EXTERNDEF Fixup_CallAddress: QWORD

.code

GameOverlay_Detour proc
	
	;Jmp To Rebuild Stolen Bytes
	;INT 3
	JMP [Fixup_CallAddress]

GameOverlay_Detour endp

end