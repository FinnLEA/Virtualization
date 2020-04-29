#include "vm.h"


void _generate_opcode_table_()
{
	//opcode_table = (uint32_t**)malloc(sizeof(uint32_t*) * INTSR_COUNT);
	//for (int i = 0; i < INTSR_COUNT; ++i)
	//	opcode_table[i] = (BYTE*)malloc(sizeof(BYTE) * 12);

//	table = OPODE_TABLE;
	
}

void _push_(vm_ptr vm, DWORD value)
{
	DWORD curr_addr = vm->REG[SP] + vm->SS;
	if (curr_addr > vm->SS + SIZE_SS) {
		vm->REG[SP] = 0x00000000;
	}
	vm->SS[vm->REG[SP]] = value;
	vm->REG[SP] += 0x00000004;
}

void _pop_(vm_ptr vm, DWORD* dst)
{
	vm->REG[SP] -= 0x00000004;
	DWORD curr_addr = vm->REG[SP] + vm->SS;
	if (curr_addr < vm->SS) {
		*dst = 0;
		vm->REG[SP] = 0x00000000;
	}
	else {
		*dst = vm->SS[vm->REG[SP]];
	}
}

uint32_t define_operand(vm_ptr vm, enum _types_ type, DWORD ex_type)
{
	// _push_(vm, (DWORD)(((DWORD)type << 8) + num_reg));
	_push_(vm, type);
	switch (type) 
	{
	case reg_:
		vm->REG[r9] = (type << 2) + ex_type;
		break;
	case imm_:
		
		break;
	case comst_:
		break;
	default:
		break;
	}
	
	return vm->REG[r9];
}

void _vm_init_(vm_ptr vm)
{
	DWORD size = 1 << (FLAG_SIZE_SS + 7);
	vm->DS = (uint32_t*)malloc(SIZE_DS);
	vm->SS = (uint32_t*)malloc(SIZE_SS);
	vm->CS = (uint32_t*)malloc(12);


	for (int i = 1; i < 7; ++i)
		vm->REG[i] = 0;

	vm->REG[SP] = 0;
	vm->REG[IP] = 0;
	vm->REG[r9] = vm->REG[SP];
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

void _vm_mov_(vm_ptr vm, OP op1, OP op2)
{
	DWORD tmp;
	_asm
	{
		lea esi, [op2]
		mov eax, [esi].value
		mov [tmp], eax
	}
	vm->REG[op1.value] = tmp;
}
