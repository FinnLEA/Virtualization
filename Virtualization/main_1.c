#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "include/pcre.h"
#include "include/pcreposix.h"

#include "protect.h"



#ifdef  _WIN64

#else


typedef void(*instrFunc)(void* pArg);

BYTE DecryptInstruction(BYTE opcode) {



}

BOOL CheckInitInstruction(BYTE opcode) {
	for (int i = 0; i < COUNT_START_OPCODES; ++i)
		if (startOpcodes[i] == opcode)
			return TRUE;

	return FALSE;
}

instrFunc ParseInstructionOpcode(vm_ptr vm, BYTE opcode, BYTE* opCount) {
	if (CheckInitInstruction(opcode)) {
		*opCount = 3;
		return (instrFunc)_vm_init_cs_;
	}

	BYTE trueOpcode = Decrypt(vm->cs, opcode >> 4);

	switch (trueOpcode)	{
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
		
	default:
		return NULL;
	}
}

BYTE ParseOperandOpcode(BYTE operandByte, OP* operandStruct) {
	return 1;
}

void ParsePref(BYTE preffix, vm_ptr vm) {

	if (preffix == 0x43) {
		return;
	}

	if (preffix == 0xe3) {
		WRITE_EIF(1);
	}

}

void ParseInstruction(PCONTEXT pContext, vm_ptr vm) {
	BYTE preffixByte, instrByte, op1Byte, op2Byte, op3Byte;
	BYTE opsCount = 0;
	instrFunc pInstr;

	preffixByte = GET_BYTE_CIP(pContext);
	if (preffixByte) {
		ParsePref(preffixByte, vm);
	}
	pContext->Cip += 0x01;
	instrByte = GET_BYTE_CIP(pContext);

	pInstr = ParseInstructionOpcode(vm, instrByte, &opsCount);
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
		pContext->Cip += 0x01;
		op1Byte = GET_BYTE_CIP(pContext);
		OP* op1 = (OP*)malloc(sizeof(OP));

		if (ParseOperandOpcode(op1Byte, op1) == 1) {
			pContext->Cip += 0x01;
		}
		else {
			pContext->Cip += 0x02;
		}
	}
	



}

int Handler(EXCEPTION_POINTERS *pException, vm_ptr vm) {
	if (pException->ExceptionRecord->ExceptionCode == STATUS_ILLEGAL_INSTRUCTION) {
		pException->ContextRecord->Cip += 0x02;
		
		BYTE opCount = 0;
		ParseInstruction(pException->ContextRecord, vm);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else {
		return EXCEPTION_EXECUTE_HANDLER;
	}
}

void func(int a, int b, int* c) {
	*c = a + b + *c;
	return;
}

int main() {

	DWORD res = 0;
_BEGIN_PROTECT_(_1_KBYTE,_256_BYTE)
{__try {
__asm ud2
__asm _emit 0x43
__asm _emit 0x05
__asm _emit 0x9c
__asm _emit 0x6e
__asm _emit 0x25
	{
		__try {
			
		}
		__except (Handler(GetExceptionInformation(), vm)) {
		}
		vm = 0;
	}

	/*PCRYPTOSYSTEM cs = init_crypto();
	BYTE res = Encrypt(cs, 0);
	res = Decrypt(cs, res);*/
	
	_BEGIN_PROTECT_(_1_KBYTE, _256_BYTE)
	{
		__try {
			__asm ud2
			__asm _emit 0x43
			__asm _emit 0x4c
			__asm _emit 0x99
			__asm _emit 0x60
			__asm _emit 0x21
			{
				define_operand(vm, 1, 1);
				define_operand(vm, 4, 2);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0x9e
				__asm _emit 0x11
				__asm _emit 0x42
				define_operand(vm, 2, 0x00000004);
				define_operand(vm, 3, 0xffffffff);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0xe8
				__asm _emit 0x02
				__asm _emit 0x04
				__asm _emit 0x03
				define_operand(vm, 1, 1);
				define_operand(vm, 2, 0x4);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0xb4
				__asm _emit 0x11
				__asm _emit 0x02
				__asm _emit 0x04
				define_operand(vm, 1, 1);
				define_operand(vm, 3, 0x2);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0x33
				__asm _emit 0x11
				__asm _emit 0x03
				define_operand(vm, 1, 1);
				define_operand(vm, 3, 0x10);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0x50
				__asm _emit 0x11
				__asm _emit 0x03
				define_operand(vm, 1, 2);
				define_operand(vm, 1, 1);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0x3b
				__asm _emit 0x12
				__asm _emit 0x11
			}
		}
		__except (Handler(GetExceptionInformation())) {
			_vm_destruct_(vm);
		}
	}
	printf("%d\n", res);
	system("pause");
	return 0;
}