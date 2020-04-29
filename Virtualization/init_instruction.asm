.686
.model flat, stdcall

.data


.code 

label_1:
	mov eax, ebx
	jmp ecx

foo proc 

	mov eax, 6

foo endp

_init_ops_ proc op1:DWORD, op2:ptr DWORD, vbp_reg:PTR DWORD

	mov eax, [op1]
	and eax, 0ff00h
	.if eax != 0
		mov ebx, [vbp_reg]
		mov ebx, [ebx]
		mov ebx, 03h
		shl ebx, 24
		mov ecx, [op1]
		and ecx, 0ffffff00h
		shr ecx, 8
	.else 
		
	.endif
_init_ops_ endp

end