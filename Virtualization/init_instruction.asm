;------------------------------------------------------------------------------
;
;		Инициализация операндов инструкций
;
;------------------------------------------------------------------------------


.686
.model flat, c


;----------------------------------------------------------
;		Consts and macroses
;----------------------------

; доступ к vm->r9 адрес в eax
_vr9 macro vm_
	
	mov ebx, vm_
	add ebx, 0ch ;vm->REG[]
	mov eax, ebx
	add eax, 40

endm

FIRST	equ 0
SECOND	equ 4


.data


.code 


;----------------------------------------------------------
;		Prototypes
;----------------------------

_push_	proto c :PTR DWORD, :DWORD
_pop_	proto c :PTR DWORD, :PTR DWORD
_get_secret_op_value_ proto c :PTR DWORD, :WORD, :PTR DWORD

;----------------------------------------------------------


;----------------------------------------------------------
;		Definitions functions
;----------------------------

_init_ops_ proc vm:ptr DWORD, op1:PTR DWORD, op2:PTR DWORD
	
	_vr9(vm)

	mov edi, [op2]
	mov ecx, [eax]
	mov dx, 2
	.while dx != 0
		lea cx, word ptr [ecx]
		.if cx >= 01000h || cx >= 04000h ; если регистр -> тип вместе с номером регистра, значение - мусором
			mov bl, ch
			mov byte ptr [edi], bl
			mov dword ptr [edi + 1], 0ffffffffh
			mov byte ptr [edi + 1], cl
			;mov ecx, [vm] 
			;mov ecx, [ecx + 8] 
			;push eax
			;push dx
			;invoke _pop_, [vm], ecx ; pop DS:0
			;pop dx
			;pop eax
		.elseif cx >= 0300h
			mov bl, ch
			shl bl, 4
			mov byte ptr [edi], bl
			lea ecx, [edi + 1]
			push eax
			push dx
			;invoke _pop_, [vm], ecx
			mov ax, 4
			sub dx, 1
			mul dx
			invoke _get_secret_op_value_, [vm], ax, ecx
			pop dx
			pop eax
		.elseif cx >= 0200h
			mov bl, ch
			shl bl, 4
			mov byte ptr [edi], bl
			lea ecx, [edi + 1]
			push eax
			push dx
			;invoke _pop_, [vm], ecx
			mov ax, 4
			sub dx, 1
			mul dx
			invoke _get_secret_op_value_, [vm], ax, ecx
			pop dx
			pop eax
			;sub [edi + 1], bl
		.endif
		mov ecx, [eax + 2]
		dec dx
		mov edi, [op1]
	.endw

	ret
_init_ops_ endp


_init_operand_ proc vm:ptr DWORD, op:PTR DWORD
	
	_vr9(vm)

	mov edi, [op]
	mov ecx, [eax]
	lea cx, word ptr [ecx]

	.if cx >= 1000h || cx >= 4000h ; если регистр -> тип вместе с номером регистра, значение - мусором
		mov bl, ch
		mov byte ptr [edi], bl
		mov dword ptr [edi + 1], 0ffffffffh
		mov byte ptr [edi + 1], cl
	.elseif cx >= 0300h
		mov bl, ch
		shl bl, 4
		mov byte ptr [edi], bl
		lea ecx, [edi + 1]
		push eax
		;invoke _pop_, [vm], ecx
		;mov ax, 4
		;mul dx
		invoke _get_secret_op_value_, [vm], FIRST, ecx
		pop eax
	.else 
		mov bl, ch
		shl bl, 4
		mov byte ptr [edi], bl
		push eax
		;invoke _pop_, [vm], [edi + 1]
		invoke _get_secret_op_value_, [vm], FIRST, ecx
		pop eax
		sub [edi + 1], bl
	.endif
	;mov ecx, [eax]
	;shr dword ptr [eax], 16

	ret

_init_operand_ endp

;----------------------------------------------------------

end