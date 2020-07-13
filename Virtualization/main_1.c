#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "include/pcre.h"
#include "include/pcreposix.h"

#include "protect.h"



int main() {

	DWORD res;

	EXCEPTION_POINTERS *pException = NULL;
	__try {
		res = 1;
		__asm {

			
		}
	}
	__except (Handler(GetExceptionInformation()))
	{
		printf("Exception handler main\n");
		printf("addr: %p\n", pException->ExceptionRecord->ExceptionAddress);
		printf("code: %08X\n", pException->ExceptionRecord->ExceptionCode);
		printf("flags: %08X\n", pException->ExceptionRecord->ExceptionFlags);
	}


	
_BEGIN_PROTECT_(_1_KBYTE,_256_BYTE)
	{
		DWORD res1;
define_operand(vm,1,1);
define_operand(vm,4,2);
__asm ud2
__asm _emit 0x00
__asm _emit 0x01
__asm _emit 0x11
__asm _emit 0x42
define_operand(vm,2,0x00000004);
define_operand(vm,3,0xffffffff);
__asm ud2
__asm _emit 0x00
__asm _emit 0x01
__asm _emit 0x02
__asm _emit 0x04
__asm _emit 0x03
define_operand(vm,1,1);
define_operand(vm,2,0x4);
__asm ud2
__asm _emit 0x00
__asm _emit 0x01
__asm _emit 0x11
__asm _emit 0x02
__asm _emit 0x04
define_operand(vm,1,1);
define_operand(vm,3,0x2);
__asm ud2
__asm _emit 0x00
__asm _emit 0x05
__asm _emit 0x11
__asm _emit 0x03
define_operand(vm,1,1);
define_operand(vm,3,0x10);
__asm ud2
__asm _emit 0x00
__asm _emit 0x03
__asm _emit 0x11
__asm _emit 0x03
define_operand(vm,1,2);
define_operand(vm,1,1);
__asm ud2
__asm _emit 0x00
__asm _emit 0x01
__asm _emit 0x12
__asm _emit 0x11
	}
	END_PROTECT(res);

	printf("%d\n", res);
	system("pause");
	return 0;
}