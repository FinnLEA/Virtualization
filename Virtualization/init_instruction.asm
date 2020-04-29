.686
.model flat, c

.data


.code 

label_1:
	mov eax, ebx
	jmp ecx

foo proc 

	mov eax, 6

foo endp

_init_ops_ proc op1:PTR DWORD, op2:PTR DWORD, vR9_reg:PTR DWORD
	mov eax, [op1]
	mov ecx, [vR9_reg]
	mov cl, byte ptr [ecx]
	mov byte ptr [eax], cl

	ret
_init_ops_ endp

end