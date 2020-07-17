#include "vm.h"


BYTE startOpcodes[COUNT_START_OPCODES] = { 0xbe, 0xac, 0x05, 0x4c, 0xb0 };

BYTE valuesForRotors[16][16] = { {0x0b, 0x07, 0x05, 0x0e, 0x0f, 0x06, 0x00, 0x0d, 0x02, 0x04, 0x0c, 0x0a, 0x09, 0x01, 0x08, 0x03, },
								{0x00, 0x0b, 0x01, 0x09, 0x07, 0x08, 0x0c, 0x0f, 0x04, 0x06, 0x0d, 0x05, 0x03, 0x0a, 0x0e, 0x02, },
								{0x09, 0x06, 0x0c, 0x00, 0x01, 0x0f, 0x0e, 0x04, 0x0b, 0x0a, 0x05, 0x08, 0x03, 0x0d, 0x02, 0x07, },
								{0x09, 0x02, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x0d, 0x0a, 0x0e, 0x0b, 0x05, 0x08, 0x0c, 0x04, 0x06, },
								{0x01, 0x0a, 0x06, 0x02, 0x0d, 0x0e, 0x09, 0x00, 0x03, 0x0c, 0x05, 0x04, 0x0b, 0x08, 0x07, 0x0f, },
								{0x07, 0x05, 0x0b, 0x0a, 0x02, 0x0f, 0x04, 0x09, 0x01, 0x06, 0x03, 0x0d, 0x0c, 0x08, 0x00, 0x0e, },
								{0x0a, 0x03, 0x0c, 0x08, 0x0d, 0x04, 0x0b, 0x05, 0x07, 0x06, 0x00, 0x0e, 0x01, 0x02, 0x09, 0x0f, },
								{0x0f, 0x04, 0x0d, 0x00, 0x05, 0x0c, 0x06, 0x0a, 0x02, 0x08, 0x07, 0x01, 0x0b, 0x09, 0x03, 0x0e, },
								{0x07, 0x04, 0x09, 0x0a, 0x0b, 0x0f, 0x03, 0x05, 0x02, 0x0c, 0x0e, 0x0d, 0x06, 0x00, 0x01, 0x08, },
								{0x06, 0x0f, 0x02, 0x08, 0x0d, 0x03, 0x0c, 0x0b, 0x05, 0x04, 0x0e, 0x09, 0x00, 0x0a, 0x01, 0x07, },
								{0x00, 0x07, 0x04, 0x03, 0x02, 0x0a, 0x0c, 0x09, 0x06, 0x01, 0x0d, 0x08, 0x0b, 0x0f, 0x05, 0x0e, },
								{0x0b, 0x08, 0x0f, 0x09, 0x06, 0x0c, 0x03, 0x0e, 0x00, 0x04, 0x05, 0x01, 0x0d, 0x07, 0x0a, 0x02, },
								{0x08, 0x05, 0x0d, 0x06, 0x0a, 0x01, 0x0f, 0x0e, 0x03, 0x09, 0x04, 0x07, 0x0b, 0x02, 0x0c, 0x00, },
								{0x0b, 0x0f, 0x09, 0x05, 0x0d, 0x04, 0x0c, 0x01, 0x03, 0x00, 0x06, 0x0e, 0x0a, 0x02, 0x07, 0x08, },
								{0x0b, 0x02, 0x00, 0x0c, 0x09, 0x0e, 0x05, 0x06, 0x0a, 0x01, 0x0f, 0x03, 0x08, 0x04, 0x0d, 0x07, },
								{0x06, 0x0b, 0x03, 0x09, 0x02, 0x0e, 0x0f, 0x00, 0x0d, 0x08, 0x01, 0x0a, 0x0c, 0x05, 0x04, 0x07, } };


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
		//_push_(vm, type); // при подготовке операндов к инструкции берем тип из вирт.стека
		if (READ_MOF) { // код предыдущего операнда - память : 2 байта (тип, последний байт вирт. адреса)
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd); // из r9 будем брать код операнда
		}
		else if (READ_COF) { // аналогично
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		else if (READ_ROF){ // код предыдущего операнда - регистр : 1-ый байт (4 ст.бита - тип операнда, 4 мл.бита - номер регистра), 2-й байт - мусор
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		else { // первый операнд в интсрукции
			vm->REG[r9] = ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		// ставим бит регистра
		WRITE_ROF(1);
		break;

	case constaddr_:
		//_push_(vm, ((BYTE)(type) + ex_type)); // при подготовке операндов к инструкции берем (значение - тип из второго байта)  
		//(vm->DS + SIZE_DS + 
		if (READ_MOF) {
			WRITE_OP_DS(SECOND, ex_type);
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) + (ex_type & 0xff));
		}
		else if (READ_COF) {
			WRITE_OP_DS(SECOND, ex_type);
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) + (ex_type & 0xff));
		}
		else if (READ_ROF) {
			WRITE_OP_DS(SECOND, ex_type);
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) + (ex_type & 0xff));
		}
		else {
			WRITE_OP_DS(FIRST, ex_type);
			vm->REG[r9] = (type << 8) + (ex_type & 0xff);
		}
		WRITE_MOF(1);
		break;

	case const_: // в вирт. стек значение; в r9 2 байта: 1-ый байт - тип операнда, 2-ый байт - мусор
		//_push_(vm, ex_type);
		if (READ_MOF) {
			WRITE_OP_DS(SECOND, ex_type);
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) | 0xff);
		}
		else if (READ_COF) {
			WRITE_OP_DS(SECOND, ex_type);
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) | 0xff);
		}
		else if (READ_ROF) {
			WRITE_OP_DS(SECOND, ex_type);
			vm->REG[r9] = (vm->REG[r9] << 16) | ((type << 8) | 0xff);
		}
		else {
			WRITE_OP_DS(FIRST, ex_type);
			vm->REG[r9] = ((type << 8) | 0xff);
		}
		WRITE_COF(1);
		break;
	case regaddr_:
		if (READ_MOF) { // код предыдущего операнда - память : 2 байта (тип, последний байт вирт. адреса)
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd); // из r9 будем брать код операнда
		}
		else if (READ_COF) { // аналогично
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		else if (READ_ROF){ // код предыдущего операнда - регистр : 1-ый байт (4 ст.бита - тип операнда, 4 мл.бита - номер регистра), 2-й байт - мусор
			vm->REG[r9] = (vm->REG[r9] << 16) | ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		else { // первый операнд в интсрукции
			vm->REG[r9] = ((((type << 4) | ex_type) << 8) | 0xcd);
		}
		// ставим бит регистра
		WRITE_ROF(1);
		break;
	default:
		break;
	}
	
	return vm->REG[r9];
}

DWORD _get_secret_op_value_(vm_ptr vm, WORD value, DWORD* dst)
{
	return *dst = *((DWORD*)((BYTE*)(vm->DS + SIZE_DS + value)));
}

void _vm_init_(vm_ptr vm)
{
	DWORD size = 1 << (FLAG_SIZE_SS + 7);
	vm->DS = (BYTE*)malloc(SIZE_DS + (sizeof(DWORD) * 2));
	vm->SS = (uint32_t*)malloc(SIZE_SS);
	vm->CS = (uint32_t*)malloc(12);


	for (int i = 1; i < 7; ++i)
		vm->REG[i] = 0;

	vm->REG[SP] = 0;
	vm->REG[IP] = 0;
	vm->REG[r9] = 0;
}



DWORD _vm_destruct_(vm_ptr vm)
{
	//free(vm->CS);
	free(vm->DS);
	free(vm->SS);
	//DWORD end_value = vm->REG[R1];
	
	for (int i = 0; i < r9; ++i)
		vm->REG[i] = 0;
	FreeCs(vm->cs);
	free(vm);
	return 0;
}

static DWORD DecodeValue(OP* operand) {
	DWORD value = 0;
	DWORD rotate = 0;
	for (int i = 0; i < 4; ++i) {
		rotate = (0xff << (i * 8));
		if ((operand->value & rotate) != 0) {
			value |= ((((operand->value & rotate) >> (i * 8)) ^ (operand->type >> 4)) << (i * 8));
		}
	}
	return value;
}

static DWORD GetValue(vm_ptr vm, OP* op){
	DWORD value = 0;
	
	switch (op->type >> 4)
	{	
	case constaddr_:
		//DecodeValue(op);
		//value = *((DWORD*)((BYTE*)(vm->DS + op->value)));
		value = *((DWORD*)((BYTE*)(vm->DS + DecodeValue(op))));
		break;

	case regaddr_:
		value = *(DWORD*)((BYTE*)(vm->DS + vm->REG[op->type & 0x0f]));
		break;

	case const_:
		//value = op->value;
		value = DecodeValue(op);
		break;

	case reg_:
		value = vm->REG[(op->type) & 0x0f];
		break;

	default:
		value = 0;
		//except
		break;
	}

	return value;
}

 static void SetValue(vm_ptr vm, OP* op, DWORD value){
 	switch (op->type >> 4)
 	{
 	case constaddr_:
 		*((DWORD*)((BYTE*)(vm->DS + DecodeValue(op)))) = value;
 		break;

 	case const_:
 		//except;
 		return;

 	case regaddr_:
 		*(DWORD*)((BYTE*)(vm->DS + vm->REG[op->type & 0x0f])) = value;
 		break;

 	case reg_:
 		vm->REG[(op->type) & 0x0f] = value;
 		break;
 	default:
 		//except
 		break;
 	}
 }

void _vm_mov_(vm_ptr vm, OP* op1, OP* op2)
{
	DWORD tmp;

	tmp = GetValue(vm, op2);
	SetValue(vm, op1, tmp);

}

void _vm_add_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 += val2;
	SetValue(vm, op1, val1);

}

void _vm_sub_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 -= val2;
	SetValue(vm, op1, val1);
}

void _vm_xor_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 ^= val2;
	SetValue(vm, op1, val1);
}

void _vm_and_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 &= val2;
	SetValue(vm, op1, val1);
}

void _vm_or_(vm_ptr vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 |= val2;
	SetValue(vm, op1, val1);
}

void _vm_push_(vm_ptr vm, OP * op1, OP * op2) {
	DWORD tmp = 0;
	tmp = GetValue(vm, op1);
	_push_(vm, tmp);
}

void _vm_pop_(vm_ptr vm, OP* op1, OP* op2) {
	DWORD tmp = 0;
	_pop_(vm, &tmp);
	SetValue(vm, op1, tmp);
}

void _vm_call_(vm_ptr vm, DWORD addr) {
	return;
}

void _vm_mul_(vm_ptr vm, OP* op1, OP* op2){
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 *= val2;
	SetValue(vm, op1, val1);
	return;
}

void _vm_div_(vm_ptr vm, OP* op1, OP* op2){
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 /= val2;
	SetValue(vm, op1, val1);
	return;
}

void _vm_init_cs_(vm_ptr vm, BYTE op1, BYTE op2, BYTE op3)
{
	BYTE startState, index;
	PSTATE State = (PSTATE)malloc(sizeof(STATE));

	State->first	= (op1 & 0xf0) >> 4;
	State->second	= (op2 & 0xf0) >> 4;
	State->third	= (op3 & 0xf0) >> 4;

	CsSetStates(vm->cs, State);

	CsSetRotors(vm->cs, valuesForRotors[op1 & 0x0f], valuesForRotors[op2 & 0x0f], valuesForRotors[op3 & 0x0f]);
	
}
