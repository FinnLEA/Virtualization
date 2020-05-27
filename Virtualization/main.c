#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#include "protect.h"


int Handler(EXCEPTION_POINTERS *pException) {
	if (pException->ExceptionRecord->ExceptionCode == STATUS_ILLEGAL_INSTRUCTION) {
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
	//__try {}
	//__except(1){// обработчик}
	//EXCEPTION_POINTERS *pException = NULL;
	//__try {
	//	res = 1;
	//	__asm {
	//		mov eax, [res]

	//		ud2
	//		_emit 0x00
	//		_emit 0xfa
	//		_emit 0x12
	//		_emit 0x33
	//	}
	//}
	//__except (Handler(GetExceptionInformation()))
	//{
	//	printf("Exception handler main\n");
	//	printf("addr: %p\n", pException->ExceptionRecord->ExceptionAddress);
	//	printf("code: %08X\n", pException->ExceptionRecord->ExceptionCode);
	//	printf("flags: %08X\n", pException->ExceptionRecord->ExceptionFlags);
	//}

	//foo();
	PCRYPTOSYSTEM cs = init_crypto();
	BYTE r = Encrypt(cs, 0x05);
	BYTE d = Decrypt(cs, r); 
	r = Encrypt(cs, 0x05);
	d = Decrypt(cs, r);
	BEGIN_PROTECT(_1_KBYTE, _256_BYTE);
	{
		DWORD res1;

		VM_MOV(IMM(0x00000004), CONST(0xffffffff));
		VM_MOV(r0, IMM(4));
		VM_ADD(r0, CONST(2));
		VM_OR(r0, CONST(0X10));
		VM_MOV(r1, r0);
	}
	END_PROTECT(res);

	printf("%d\n", res);
	system("pause");
	return 0;
}