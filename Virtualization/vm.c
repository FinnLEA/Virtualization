/*-----------------------------------------------------------------------------
	
	Реализация внутренней архитектуры виртуальной машины

-----------------------------------------------------------------------------*/


#include "include\vm.h"


//---------------------------------------------------------
/*		Global variables		*/
//---------------------------

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

//	
BYTE staticOpcodes[16][COUNT_INSTRUCTIONS] = { {0x0b, 0x6a, 0xb6, 0x92, 0xf0, 0x3f, 0x82, 0x34, 0x32, 0x08, 0xdf, },
							{0x3a, 0x7a, 0x2c, 0x9c, 0x17, 0xa9, 0xa8, 0x6a, 0x9b, 0xe5, 0xdc, },
							{0xae, 0xc4, 0x09, 0x18, 0xe3, 0x7d, 0x85, 0xf8, 0x8d, 0x0f, 0x3d, },
							{0x10, 0x24, 0xc3, 0xb6, 0x53, 0xe0, 0x46, 0x4f, 0xd8, 0x00, 0xfc, },
							{0xcd, 0x0a, 0xcb, 0x14, 0xa1, 0x27, 0x38, 0x32, 0x17, 0x98, 0xff, },
							{0xf9, 0x02, 0x3a, 0x77, 0x31, 0x5d, 0x55, 0xce, 0xe7, 0xbb, 0x8d, },
							{0xb4, 0xe9, 0x24, 0xe5, 0xcb, 0xd6, 0x35, 0xfd, 0xd4, 0x58, 0xbd, },
							{0x3c, 0x16, 0x52, 0x14, 0xea, 0x71, 0x01, 0x12, 0x17, 0xcb, 0x6f, },
							{0x61, 0xdf, 0x86, 0x3c, 0x11, 0x1e, 0x81, 0x5e, 0x90, 0xc9, 0xb1, },
							{0xd3, 0x5c, 0x6d, 0x1c, 0x42, 0x97, 0x03, 0x8f, 0x58, 0x01, 0xf9, },
							{0xf1, 0xe3, 0x65, 0x4e, 0x15, 0xd5, 0x7f, 0xbd, 0xc6, 0xb8, 0x03, },
							{0x2e, 0xe4, 0xa7, 0xcc, 0xfe, 0x32, 0x1d, 0x39, 0xe6, 0x9d, 0x02, },
							{0x33, 0xa6, 0x87, 0x6a, 0x56, 0x53, 0x9c, 0xe5, 0x51, 0x16, 0x3f, },
							{0x51, 0xd1, 0x4a, 0x0b, 0xa0, 0x30, 0x79, 0xf5, 0x0c, 0xde, 0xd0, },
							{0xfa, 0xae, 0xaf, 0x53, 0x56, 0x2c, 0xa5, 0xe2, 0xf2, 0x7f, 0x16, }, };


BYTE staticOpTypes[16][4] = { {0x02, 0x0c, 0x03, 0x00, },
							{0x0a, 0x01, 0x0f, 0x0c, },
							{0x07, 0x06, 0x08, 0x0e, },
							{0x0c, 0x0f, 0x03, 0x07, },
							{0x01, 0x08, 0x03, 0x09, },
							{0x08, 0x0e, 0x0b, 0x05, },
							{0x0f, 0x00, 0x08, 0x0d, },
							{0x0a, 0x0d, 0x0b, 0x0f, },
							{0x0d, 0x0f, 0x0e, 0x08, },
							{0x05, 0x06, 0x0a, 0x0e, },
							{0x07, 0x0a, 0x0f, 0x0e, },
							{0x01, 0x05, 0x0f, 0x0e, },
							{0x04, 0x0a, 0x01, 0x07, },
							{0x03, 0x05, 0x09, 0x00, },
							{0x08, 0x0d, 0x06, 0x0c, }, };

//---------------------------------------------------------


//---------------------------------------------------------
/*		Definitions functions		*/
//---------------------------


BYTE FindInOpcodeTable(BYTE opcode, BYTE index) {
	for (BYTE i = 0; i < COUNT_INSTRUCTIONS; ++i) {
		if (opcode == staticOpcodes[index][i])
			return i + 1;
	}
	return 0;
}

BYTE FindInOpTypeTable(BYTE operandByte, BYTE index) {
	for (BYTE i = 0; i < 4; ++i) {
		if (operandByte == staticOpTypes[index][i])
			return i + 1;
	}
	return 0;
}

void _push_(VM_PTR vm, DWORD value)
{
	DWORD curr_addr = vm->REG[SP] + vm->SS;
	if (curr_addr > vm->SS + SIZE_SS) {
		vm->REG[SP] = 0x00000000;
	}
	vm->SS[vm->REG[SP] / 4] = value;
	vm->REG[SP] += 0x00000004;
}

void _pop_(VM_PTR vm, DWORD* dst)
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

uint32_t define_operand(VM_PTR vm, enum _types_ type, DWORD ex_type)
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

DWORD _get_secret_op_value_(VM_PTR vm, WORD value, DWORD* dst)
{
	return *dst = *((DWORD*)((BYTE*)(vm->DS + SIZE_DS + value)));
}

void _vm_init_(VM_PTR vm)
{
	//DWORD size = 1 << (FLAG_SIZE_SS + 7);
	vm->DS = (BYTE*)malloc(SIZE_DS + (sizeof(DWORD) * 2));
	vm->SS = (uint32_t*)malloc(SIZE_SS);
	vm->CS = (uint32_t*)malloc(12);

	vm->pInstrForLoop = (PINSTR_LOOP)malloc(sizeof(INSTR_LOOP));

	for (int i = 1; i < COUNT_REGS; ++i)
		vm->REG[i] = 0;

	vm->REG[SP] = 0;
	vm->REG[IP] = 0;
	vm->REG[r9] = 0;

	vm->realAddrCurrInstr = 0U;
}

void ClearStructIntsrForLoop(VM_PTR vm) {
	vm->pInstrForLoop->opcode = 0u;
	vm->pInstrForLoop->operandsTypes = (WORD)0;
}

DWORD _vm_destruct_(VM_PTR vm)
{
	//free(vm->CS);
	free(vm->DS);
	free(vm->SS);
	DWORD end_value = vm->REG[R1];
	printf("%d\n", vm->REG[R1]);
	
	for (int i = 0; i < r9; ++i)
		vm->REG[i] = 0;
	//FreeCs(vm->cs);
	free(vm);
	return end_value;
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

static DWORD GetValue(VM_PTR vm, OP* op){
	DWORD value = 0;
	
	switch (op->type >> 4)
	{	
	case constaddr_:
		//DecodeValue(op);
		//value = *((DWORD*)((BYTE*)(vm->DS + op->value)));
		value = *((DWORD*)((BYTE*)(vm->DS + DecodeValue(op))));
		break;

	case regaddr_:
		value = *((DWORD*)((BYTE*)(vm->DS + vm->REG[op->type & 0x0f])));
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

 static void SetValue(VM_PTR vm, OP* op, DWORD value){
 	switch (op->type >> 4)
 	{
 	case constaddr_:
 		*((DWORD*)((BYTE*)(vm->DS + DecodeValue(op)))) = value;
 		break;

 	case const_:
 		//except;
 		return;

 	case regaddr_:
 		*((DWORD*)((BYTE*)(vm->DS + vm->REG[op->type & 0x0f]))) = value;
 		break;

 	case reg_:
 		vm->REG[(op->type) & 0x0f] = value;
 		break;
 	default:
 		//except
 		break;
 	}
 }

void _vm_mov_(VM_PTR vm, OP* op1, OP* op2)
{
	DWORD tmp;

	tmp = GetValue(vm, op2);
	SetValue(vm, op1, tmp);

}

void _vm_add_(VM_PTR vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 += val2;
	SetValue(vm, op1, val1);

}

void _vm_sub_(VM_PTR vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 -= val2;
	SetValue(vm, op1, val1);
}

void _vm_xor_(VM_PTR vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 ^= val2;
	SetValue(vm, op1, val1);
}

void _vm_and_(VM_PTR vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 &= val2;
	SetValue(vm, op1, val1);
}

void _vm_or_(VM_PTR vm, OP * op1, OP * op2)
{
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 |= val2;
	SetValue(vm, op1, val1);
}

void _vm_push_(VM_PTR vm, OP * op1, OP * op2) {
	DWORD tmp = 0;
	tmp = GetValue(vm, op1);
	_push_(vm, tmp);
}

void _vm_pop_(VM_PTR vm, OP* op1, OP* op2) {
	DWORD tmp = 0;
	_pop_(vm, &tmp);
	SetValue(vm, op1, tmp);
}

void _vm_call_(VM_PTR vm, OP* op1, OP* op2) {
	return;
}

void _vm_mul_(VM_PTR vm, OP* op1, OP* op2){
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 *= val2;
	SetValue(vm, op1, val1);
	return;
}

void _vm_div_(VM_PTR vm, OP* op1, OP* op2){
	DWORD val1, val2;
	val1 = GetValue(vm, op1);
	val2 = GetValue(vm, op2);

	val1 /= val2;
	SetValue(vm, op1, val1);
	return;
}

void _vm_init_cs_(VM_PTR vm, BYTE op1, BYTE op2, BYTE op3)
{
	BYTE startState, index;
	PSTATE State = (PSTATE)malloc(sizeof(STATE));

	State->first	= (op1 & 0xf0) >> 4;
	State->second	= (op2 & 0xf0) >> 4;
	State->third	= (op3 & 0xf0) >> 4;

	CsSetStates(vm->cs, State);

	CsSetRotors(vm->cs, valuesForRotors[op1 & 0x0f], valuesForRotors[op2 & 0x0f], valuesForRotors[op3 & 0x0f]);
	
}


//---------------------------
//
//	Instructions parse and handle functions	
//

static void DecryptInstruction(VM_PTR vm, BYTE opcode, BYTE* retOpcode) {

	if (READ_IEF == 1) { // if enigma code
		if (READ_LEF == 0)
			*retOpcode = Decrypt(vm->cs, opcode >> 4);
		else if (READ_LEF == 1 && ExistInstrInCache(vm->realAddrCurrInstr) == 0) {
			*retOpcode = Decrypt(vm->cs, opcode >> 4);
			vm->pInstrForLoop->opcode = *retOpcode;
		}
		else
			*retOpcode = GetOpcodeFromCache(vm->realAddrCurrInstr);
	}
	else { // if static code
		*retOpcode = FindInOpcodeTable(opcode, (BYTE)vm->REG[r10]);
	}

	return;//Decrypt(opcode);
}

static BOOL CheckInitInstruction(BYTE opcode) {
	for (int i = 0; i < COUNT_START_OPCODES; ++i)
		if (startOpcodes[i] == opcode)
			return TRUE;

	return FALSE;
}

static instrFunc ParseInstructionOpcode(VM_PTR vm, BYTE opcode, BYTE* opCount) {
	if (CheckInitInstruction(opcode)) {
		*opCount = 3;
		return (instrFunc)_vm_init_cs_;
	}
	BYTE trueOpcode;
	DecryptInstruction(vm, opcode, &trueOpcode);
	switch (trueOpcode) {
	case 0x01:
		*opCount = 2;
		return (instrFunc)_vm_mov_;
	case 0x02:
		*opCount = 2;
		return (instrFunc)_vm_and_;
	case 0x03:
		*opCount = 2;
		return (instrFunc)_vm_or_;
	case 0x04:
		*opCount = 1;
		return (instrFunc)_vm_call_;
	case 0x05:
		*opCount = 2;
		return (instrFunc)_vm_add_;
	case 0x06:
		*opCount = 2;
		return (instrFunc)_vm_sub_;
	case 0x07:
		*opCount = 2;
		return (instrFunc)_vm_xor_;
	case 0x08:
		*opCount = 2;
		return (instrFunc)_vm_mul_;
	case 0x09:
		*opCount = 2;
		return (instrFunc)_vm_div_;
	case 0x0a:
		*opCount = 1;
		return (instrFunc)_vm_push_;
	case 0x0b:
		*opCount = 1;
		return (instrFunc)_vm_pop_;

	default:
		return NULL;
	}
}

static BYTE ParseOperandOpcode(VM_PTR vm, BYTE operandByte, BYTE countOp) {
	//DecryptOperand
	BYTE trueType;
	DWORD ex_type;
	BYTE retValue = 0;

	if (READ_IEF == 1) {
		if (READ_LEF == 0) {
			trueType = Decrypt(vm->cs, operandByte >> 4);
		}
		else if (READ_LEF == 1 && ExistInstrInCache(vm->realAddrCurrInstr) == FALSE) {
			trueType = Decrypt(vm->cs, operandByte >> 4);
			vm->pInstrForLoop->operandsTypes <<= 8;
			vm->pInstrForLoop->operandsTypes |= (WORD)trueType;
		}
		else
			trueType = GetOperandTypeFromCache(vm->realAddrCurrInstr, countOp);
	}
	else {
		trueType = FindInOpTypeTable(operandByte >> 4, vm->REG[r10]);
	}

	switch ((enum _types_)trueType)
	{
	case reg_:
	case regaddr_:
		define_operand(vm, trueType, operandByte & 0x0f);
		retValue = 1;
		break;
	case constaddr_:
		ex_type = 0;
		_pop_(vm, &ex_type);
		define_operand(vm, trueType, ex_type);
		retValue = 2;
		break;
	case const_:
		ex_type = 0;
		_pop_(vm, &ex_type);
		define_operand(vm, trueType, ex_type);
		retValue = 1;
		break;

	default:
		// except
		return retValue;
	}

	return retValue;
}

static void ParsePref(BYTE preffix, VM_PTR vm) {
	//for (int i = 0; i < COUNT_PREF_CHANGE; ++i) {
	//	//
	//}
	switch (preffix)
	{
	case 0x43: // Enigma inint instruction
		return;

	case 0xe3: // Enigma just instructon
		WRITE_IEF(1);
		return;

	case 0xe1: // Enigma instructon with cycle
		WRITE_LEF(1);
		WRITE_IEF(1);
		return;

	default: // Static table 
		WRITE_IEF(0);
		vm->REG[r10] = preffix & 0x0f;
		break;
	}

}


static void ParseInstruction(PCONTEXT pContext, VM_PTR vm) {
	BYTE preffixByte, instrByte, op1Byte, op2Byte, op3Byte;
	BYTE opsCount = 0;
	instrFunc pInstr;
	vm->realAddrCurrInstr = (DWORD)pContext->Cip;
	ClearStructIntsrForLoop(vm);
	//pContext->Cip += 0x02;
	preffixByte = GET_BYTE_CIP(pContext);
	if (preffixByte) {
		// Handle pref
		ParsePref(preffixByte, vm);
	}
	pContext->Cip += 0x01;
	instrByte = GET_BYTE_CIP(pContext);

	pInstr = ParseInstructionOpcode(vm, instrByte, &opsCount);
	if (!pInstr) {
		// error
		;
		return;
	}

	if (pInstr == _vm_init_cs_) {
		pContext->Cip += 0x01;
		op1Byte = GET_BYTE_CIP(pContext);
		pContext->Cip += 0x01;
		op2Byte = GET_BYTE_CIP(pContext);
		pContext->Cip += 0x01;
		op3Byte = GET_BYTE_CIP(pContext);
		pContext->Cip += 0x01;
		pInstr(vm, op1Byte, op2Byte, op3Byte);
	}
	else {
		vm->pInstrForLoop->opCount = opsCount;
		pContext->Cip += 0x01;

		OP* op1 = (OP*)malloc(sizeof(OP));
		OP* op2 = (OP*)malloc(sizeof(OP));
		BYTE retParse;

		//switch (opsCount)
		//{
		//case 2:
		//	op2Byte = GET_BYTE_CIP(pContext);
		//	retParse = ParseOperandOpcode(vm, op2Byte); // return - количетство байт, занимаемых операндом
		//	if (retParse == 1) {
		//		pContext->Cip += 0x01;
		//	}
		//	else if (retParse == 2) {
		//		pContext->Cip += 0x02;
		//	}
		//	else {
		//		//error parse;
		//		;
		//	}
		//	_init_operand_(vm, op2);
		//case 1:
		//	op1Byte = GET_BYTE_CIP(pContext);
		//	BYTE retParse = ParseOperandOpcode(vm, op1Byte);
		//	if (retParse == 1) {
		//		pContext->Cip += 0x01;
		//	}
		//	else if (retParse == 2) {
		//		pContext->Cip += 0x02;
		//	}
		//	else {
		//		//error parse;
		//		;
		//	}
		//	_init_operand_(vm, op1);
		//	break;
		//default:
		//	break;
		//}

		op1Byte = GET_BYTE_CIP(pContext);
		retParse = ParseOperandOpcode(vm, op1Byte, 1);
		if (retParse == 1) {
			pContext->Cip += 0x01;
		}
		else if (retParse == 2) {
			pContext->Cip += 0x02;
		}
		else {
			//error parse;
			;
		}
		// если операндов 2
		if (opsCount == 2) {
			op2Byte = GET_BYTE_CIP(pContext);

			BYTE retParse = ParseOperandOpcode(vm, op2Byte, 2); // return - количетство байт, занимаемых операндом
			if (retParse == 1) {
				pContext->Cip += 0x01;
			}
			else if (retParse == 2) {
				pContext->Cip += 0x02;
			}
			else {
				//error parse;
				;
			}
			_init_ops_(vm, op1, op2);
		}
		else {
			_init_operand_(vm, op1);
		}
		if (READ_LEF == 1 && ExistInstrInCache(vm->realAddrCurrInstr) == FALSE)
			RememberInstructionInCache(vm->realAddrCurrInstr, vm->pInstrForLoop);
		pInstr(vm, op1, op2);
	}
	WRITE_MOF(0);
	WRITE_COF(0);
	WRITE_ROF(0);
	WRITE_IEF(0);
	WRITE_LEF(0);
	vm->REG[r9] = 0;
	vm->REG[r10] = 0;
}

//int count = 0;

int Handler(EXCEPTION_POINTERS* pException, VM_PTR vm) {
	printf("Exception handler main\n");
	printf("addr: %p\n", pException->ExceptionRecord->ExceptionAddress);
	printf("code: %08X\n", pException->ExceptionRecord->ExceptionCode);
	printf("flags: %08X\n", pException->ExceptionRecord->ExceptionFlags);
	
	if (pException->ExceptionRecord->ExceptionCode == STATUS_ILLEGAL_INSTRUCTION ||
		pException->ExceptionRecord->ExceptionCode == STATUS_PRIVILEGED_INSTRUCTION) {
		//printf("preff except %x%x", GET_BYTE_CIP(pException->ContextRecord), (BYTE)pException->ContextRecord->Cip + 1);
		pException->ContextRecord->Cip += 0x10;
	//	printf(++count);
		ParseInstruction(pException->ContextRecord, vm);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else {
		return EXCEPTION_EXECUTE_HANDLER;
	}
}

//---------------------------------------------------------