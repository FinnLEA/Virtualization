#include <stdlib.h>
#include <stdio.h>

#include "protect.h"

int main() {

	DWORD res;
	// Seh
	//__try {}
	//__except(1){// обработчик}
	//__asm _emit 01
	//foo();
	init_crypto();
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