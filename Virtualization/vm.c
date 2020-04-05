#include "vm.h"


void _vm_init_(vm_ptr vm)
{
	vm->DS = (uint32_t*)malloc(512);
	vm->SS = (uint32_t*)malloc(128);
	vm->CS = (uint32_t*)malloc(12);


	for (int i = 0; i < 7; ++i)
		vm->REG[i] = 0;

	vm->REG[SP] = vm->SS[0];
	vm->REG[IP] = vm->CS[0];
	vm->REG[BP] = vm->REG[SP];
}

DWORD _vm_destruct_(vm_ptr vm)
{
	free(vm->CS);
	free(vm->DS);
	free(vm->SS);
	DWORD end_value = vm->REG[R0];
	
	for (int i = 0; i < FLAGS; ++i)
		vm->REG[i] = 0;
	return end_value;
}

void _vm_mov_(vm_ptr vm, BYTE op1, OP op2)
{
	DWORD tmp;
	_asm
	{
		lea esi, [op2]
		mov eax, [esi].value
		mov [tmp], eax
	}
	vm->REG[op1] = tmp;
}
