#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "include/pcre.h"
#include "include/pcreposix.h"

#include "protect.h"


//
//int Handler(EXCEPTION_POINTERS *pException) {
//	if (pException->ExceptionRecord->ExceptionCode == STATUS_ILLEGAL_INSTRUCTION) {
//		pException->ContextRecord->Eip += 0x06;
//		//pException->ContextRecord->Ecx = 1;
//		return EXCEPTION_CONTINUE_EXECUTION;
//	}
//	else {
//		return EXCEPTION_EXECUTE_HANDLER;
//	}
//}

int main() {

	DWORD res;
	// Seh

	EXCEPTION_POINTERS *pException = NULL;
	__try {
		res = 1;
		__asm {

			ud2 // 0x0f 0x0b
			
			//DD 0x007b1fad
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


	BEGIN_PROTECT(_1_KBYTE, _256_BYTE);
	{
		DWORD res1;
		VM_MOV(r0, r1);
		VM_MOV(IMM(0x00000004), CONST(0xffffffff));
		VM_MOV(r0, IMM(4));
		VM_ADD(r0, CONST(2));
		VM_OR(r0, CONST(0x10));
		VM_MOV(r1, r0);
	}
	END_PROTECT(res);

	printf("%d\n", res);
	system("pause");
	return 0;
}