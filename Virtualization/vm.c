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
	vm->SS[vm->REG[SP] / 4] = value;
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
		*dst = vm->SS[vm->REG[SP] / 4];
	}
}

uint32_t define_operand(vm_ptr vm, enum _types_ type, DWORD ex_type)
{
	// _push_(vm, (DWORD)(((DWORD)type << 8) + num_reg));
	switch (type) 
	{
	case reg_:
		_push_(vm, type); // ��� ���������� ��������� � ���������� ����� ��� �� ����.�����
		if (READ_MOF) { // ��� ����������� �������� - ������ : 2 ����� (���, ��������� ���� ����. ������)
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd); // �� r9 ����� ����� ��� ��������
		}
		else if (READ_COF) { // ����������
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		else if (READ_ROF){ // ��� ����������� �������� - ������� : 1-�� ���� (4 ��.���� - ��� ��������, 4 ��.���� - ����� ��������), 2-� ���� - �����
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		else { // ������ ������� � ����������
			vm->REG[r9] = ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		// ������ ��� ��������
		WRITE_ROF(1);
		break;

	case imm_:
		_push_(vm, ((BYTE)(type) + ex_type)); // ��� ���������� ��������� � ���������� ����� (�������� - ��� �� ������� �����)  
		if (READ_MOF) {
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) + (ex_type & 0xff));
		}
		else if (READ_COF) {
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) + (ex_type & 0xff));
		}
		else if (READ_ROF) {
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) + (ex_type & 0xff));
		}
		else {
			vm->REG[r9] = (type << 8) + (ex_type & 0xff);
		}
		WRITE_MOF(1);
		break;

	case const_: // � ����. ���� ��������; � r9 2 �����: 1-�� ���� - ��� ��������, 2-�� ���� - �����
		_push_(vm, ex_type);
		if (READ_MOF) {
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) | 0xff);
		}
		else if (READ_COF) {
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) | 0xff);
		}
		else if (READ_ROF) {
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) | 0xff);
		}
		else {
			vm->REG[r9] = ((type << 8) | 0xff);
		}
		WRITE_COF(1);
		break;

	default:
		break;
	}
	
	return vm->REG[r9];
}

void _vm_init_(vm_ptr vm)
{
	DWORD size = 1 << (FLAG_SIZE_SS + 7);
	vm->DS = (BYTE*)malloc(SIZE_DS);
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
	
	for (int i = 0; i < r9; ++i)
		vm->REG[i] = 0;
	return end_value;
}

void _vm_mov_(vm_ptr vm, OP* op1, OP* op2)
{
	DWORD tmp;
	switch (op2->type)
	{	
	case imm_:
		tmp = *((DWORD*)((BYTE*)(vm->DS + op2->value)));
		break;

	case const_:
		tmp = op2->value;
		break;

	default:
		tmp = vm->REG[(op2->type) & 0x0f];
		break;
	}

	switch (op1->type)
	{
	case imm_:
		*((DWORD*)((BYTE*)(vm->DS + op1->value))) = tmp;
		break;

	case const_:
		return;

	default:
		vm->REG[(op1->type) & 0x0f] = tmp;
		break;
	}

}

void _vm_add_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD tmp;
	switch (op2->type)
	{
	case imm_:
		tmp = *((DWORD*)((BYTE*)(vm->DS + op2->value)));
		break;

	case const_:
		tmp = op2->value;
		break;

	default:
		tmp = vm->REG[(op2->type) & 0x0f];
		break;
	}

	switch (op1->type)
	{
	case imm_:
		*((DWORD*)((BYTE*)(vm->DS + op1->value))) += tmp;
		break;

	case const_:
		return;

	default:
		vm->REG[(op1->type) & 0x0f] += tmp;
		break;
	}
}

void _vm_sub_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD tmp;
	switch (op2->type)
	{
	case imm_:
		tmp = *((DWORD*)((BYTE*)(vm->DS + op2->value)));
		break;

	case const_:
		tmp = op2->value;
		break;

	default:
		tmp = vm->REG[(op2->type) & 0x0f];
		break;
	}

	switch (op1->type)
	{
	case imm_:
		*((DWORD*)((BYTE*)(vm->DS + op1->value))) -= tmp;
		break;

	case const_:
		return;

	default:
		vm->REG[(op1->type) & 0x0f] -= tmp;
		break;
	}
}

void _vm_xor_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD tmp;
	switch (op2->type)
	{
	case imm_:
		tmp = *((DWORD*)((BYTE*)(vm->DS + op2->value)));
		break;

	case const_:
		tmp = op2->value;
		break;

	default:
		tmp = vm->REG[(op2->type) & 0x0f];
		break;
	}

	switch (op1->type)
	{
	case imm_:
		*((DWORD*)((BYTE*)(vm->DS + op1->value))) ^= tmp;
		break;

	case const_:
		return;

	default:
		vm->REG[(op1->type) & 0x0f] ^= tmp;
		break;
	}
}

void _vm_and_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD tmp;
	switch (op2->type)
	{
	case imm_:
		tmp = *((DWORD*)((BYTE*)(vm->DS + op2->value)));
		break;

	case const_:
		tmp = op2->value;
		break;

	default:
		tmp = vm->REG[(op2->type) & 0x0f];
		break;
	}

	switch (op1->type)
	{
	case imm_:
		*((DWORD*)((BYTE*)(vm->DS + op1->value))) &= tmp;
		break;

	case const_:
		return;

	default:
		vm->REG[(op1->type) & 0x0f] &= tmp;
		break;
	}
}

void _vm_or_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD tmp;
	switch (op2->type)
	{
	case imm_:
		tmp = *((DWORD*)((BYTE*)(vm->DS + op2->value)));
		break;

	case const_:
		tmp = op2->value;
		break;

	default:
		tmp = vm->REG[(op2->type) & 0x0f];
		break;
	}

	switch (op1->type)
	{
	case imm_:
		*((DWORD*)((BYTE*)(vm->DS + op1->value))) |= tmp;
		break;

	case const_:
		return;

	default:
		vm->REG[(op1->type) & 0x0f] |= tmp;
		break;
	}
}
