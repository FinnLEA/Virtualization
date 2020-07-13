#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "include/pcre.h"
#include "include/pcreposix.h"

#include "protect.h"

#define GET_BYTE_EIP(pExc)	(BYTE)*((DWORD*)pExc->ContextRecord->Eip)

typedef void(*instrFunc)(void* pArg);

instrFunc ParseInstructionOpcode(BYTE opcode, BYTE* opCount) {
	switch (opcode)	{
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

int Handler(EXCEPTION_POINTERS *pException) {
	if (pException->ExceptionRecord->ExceptionCode == STATUS_ILLEGAL_INSTRUCTION) {
		pException->ContextRecord->Eip += 0x02;
		BYTE currByte = (BYTE)*((DWORD*)pException->ContextRecord->Eip);
		BYTE opCount = 0;
		if (!currByte)
			;
		pException->ContextRecord->Eip += 0x01;
		currByte = GET_BYTE_EIP(pException);
		instrFunc pInstr = ParseInstructionOpcode(currByte);
		
		
		pException->ContextRecord->Eip += 0x06;
		//pException->ContextRecord->Ecx = 1;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else {
		return EXCEPTION_EXECUTE_HANDLER;
	}
}

int main() {

	DWORD res;
	// Seh

	EXCEPTION_POINTERS *pException = NULL;
	__try {
		res = 1;
		__asm {
			__asm ud2
			__asm _emit 0x00
			__asm _emit 0x01
			__asm _emit 0x02
			__asm _emit 0x04
			__asm _emit 0x03
		}
	}
	__except (Handler(GetExceptionInformation()))
	{
		printf("Exception handler main\n");
		printf("addr: %p\n", pException->ExceptionRecord->ExceptionAddress);
		printf("code: %08X\n", pException->ExceptionRecord->ExceptionCode);
		printf("flags: %08X\n", pException->ExceptionRecord->ExceptionFlags);
	}

	//foo();

	//
	//BEGIN_PROTECT _1_KBYTE, _256_BYTE
	//{
	//	DWORD res1;
	//	VM_MOV r0, [r1;
	//	VM_MOV [0x00000004], ffffffff
	//	VM_MOV r0, [4]
	//	VM_ADD r0, 2
	//	VM_OR r0, 0x10
	//	VM_MOV r1, r0
	//}
	//END_PROTECT(res);

	printf("%d\n", res);
	system("pause");
	return 0;
}