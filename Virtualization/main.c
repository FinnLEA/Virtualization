#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "include/pcre.h"
#include "include/pcreposix.h"

#include "protect.h"


//BEGIN_PROTECT _1_KBYTE, _256_BYTE
//{
//	DWORD res1;
//	VM_MOV r0, r1;
//	VM_MOV [0x123454], ffffffff
//	VM_MOV r0, [4]
//	VM_ADD r0, 2
//	VM_OR r0, 0x10
//	VM_MOV r1, r0
//}
//END_PROTECT(res);

#ifdef  _WIN64

#else


#endif //  _WIN64
typedef void(*instrFunc)(void* pArg);

BYTE DecryptInstruction(BYTE opcode) {



	return 1;//Decrypt(opcode);
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
	BYTE trueOpcode;
	if (READ_EIF == 1) {
		trueOpcode = Decrypt(vm->cs, opcode >> 4);
	}
	else {
		; // not Enigma
	}
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

BYTE ParseOperandOpcode(vm_ptr vm, BYTE operandByte) {
	//DecryptOperand
	BYTE trueType;
	DWORD ex_type;
	BYTE retValue = 0;
	
	if (READ_EIF == 1) {
		trueType = Decrypt(vm->cs, operandByte >> 4);
	}
	else {
		;//
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
		return retValue;
	}

	return retValue;
}

void ParsePref(BYTE preffix, vm_ptr vm) {
	//for (int i = 0; i < COUNT_PREF_CHANGE; ++i) {
	//	//
	//}

	// Enigma inint instruction
	if (preffix == 0x43) {
		return;
	}

	// Enigma just instructon
	if (preffix == 0xe3) {
		WRITE_EIF(1);
	}

}

//#define _2_	2
//#define _1_ 1
//#define PARSE_OPERAND(count)	\
//	opcountbyte

void ParseInstruction(PCONTEXT pContext, vm_ptr vm) {
	BYTE preffixByte, instrByte, op1Byte, op2Byte, op3Byte;
	BYTE opsCount = 0;
	instrFunc pInstr;

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
		retParse = ParseOperandOpcode(vm, op1Byte);
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
			
			BYTE retParse = ParseOperandOpcode(vm, op2Byte); // return - количетство байт, занимаемых операндом
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
		pInstr(vm, op1, op2);
	}
	WRITE_MOF(0); 
	WRITE_COF(0); 
	WRITE_ROF(0); 
	WRITE_EIF(0);
	vm->REG[r9] = 0;
}

int Handler(EXCEPTION_POINTERS *pException, vm_ptr vm) {
	if (pException->ExceptionRecord->ExceptionCode == STATUS_ILLEGAL_INSTRUCTION) {
		pException->ContextRecord->Cip += 0x02;
		
		BYTE opCount = 0;
		//if (!currByte)
		//	;
		//pException->ContextRecord->Eip += 0x01;
		//currByte = GET_BYTE_EIP(pException);
		//// instrFunc pInstr = ParseInstructionOpcode(currByte);
		//
		//
		//pException->ContextRecord->Eip += 0x06;
		//pException->ContextRecord->Ecx = 1;
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

	//DWORD res = 0;

	_BEGIN_PROTECT_(_128_BYTE, _256_BYTE)
	{
		__try {
			__asm ud2
			__asm _emit 0x43
			__asm _emit 0xb0
			__asm _emit 0xb3
			__asm _emit 0xd7
			__asm _emit 0x33
			{
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0xee
				__asm _emit 0xf1
				__asm _emit 0xf2
				_push_(vm, 120);
				_push_(vm, 0x00000006);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0x36
				__asm _emit 0xcd
				__asm _emit 0x04
				__asm _emit 0x9b
				_push_(vm, 0xfcfcfcfd);
				_push_(vm, 0x00000006);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0x73
				__asm _emit 0x9f
				__asm _emit 0x04
				__asm _emit 0x60
				_push_(vm, 1);
				__asm ud2
				__asm _emit 0xe3
				__asm _emit 0xad
				__asm _emit 0x06
				__asm _emit 0x6a
			}
		}
		__except (Handler(GetExceptionInformation(), vm)) {
			_vm_destruct_(vm);
		}
		vm = 0;
	}

	/*PCRYPTOSYSTEM cs = init_crypto();
	CsSetRotors(cs, valuesForRotors[9], valuesForRotors[7], valuesForRotors[0x0b]);
	PSTATE State = (PSTATE)malloc(sizeof(STATE));

	State->first = 0x04;
	State->second = 0x08;
	State->third = 0x05;

	CsSetStates(cs, State);
	for (BYTE i = 0; i < 16; ++i) {
		BYTE res = Encrypt(cs, i);
		printf("i = %x -> %x", i, res);
		res = Decrypt(cs, res);
		printf(" -> %x\n", res);
	}*/
	DWORD res;
	BEGIN_PROTECT _32_KBYTE, _256_BYTE;
	{
		
		_While(1)
			
		_Endw
		
		VM_MOV r0, [r1];
		VM_MOV [0x00000004], 123
		VM_MOV [0x00000004], 0xfffffffe
		//VM_MOV r0, [4]
		VM_ADD r5, 2
		//VM_OR r0, 0x10
		//VM_MOV r1, r0
	}
	END_PROTECT(res);

//	printf("%d\n", res);
	system("pause");
	return 0;
}