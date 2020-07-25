#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>


#include "include\protect.h"




#ifdef  _WIN64

#else




int main() {

	DWORD res = 0;








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

	_BEGIN_PROTECT_(_32_KBYTE, _256_BYTE)
	{
		__try {
			__asm _emit 0xf2
			__asm _emit 0x65
			__asm _emit 0x36
			__asm _emit 0x3e
			__asm _emit 0xf3
			__asm _emit 0x64
			__asm _emit 0x0f
			__asm _emit 0x26
			__asm _emit 0xf0
			__asm _emit 0x66
			__asm _emit 0x2e
			__asm _emit 0x67
			__asm _emit 0x9b
			__asm _emit 0x67
			__asm _emit 0x66
			__asm _emit 0xf0

			__asm _emit 0x43
			__asm _emit 0x05
			__asm _emit 0xe7
			__asm _emit 0xba
			__asm _emit 0x39
			{

				_If(1)
					_push_(vm, 0);
				__asm _emit 0x9b
				__asm _emit 0x26
				__asm _emit 0xf3
				__asm _emit 0x2e
				__asm _emit 0x66
				__asm _emit 0x0f
				__asm _emit 0x67
				__asm _emit 0xf2
				__asm _emit 0x65
				__asm _emit 0x64
				__asm _emit 0xf0
				__asm _emit 0x3e
				__asm _emit 0x36
				__asm _emit 0x65
				__asm _emit 0x64
				__asm _emit 0xf0

				__asm _emit 0x44
				__asm _emit 0xcd
				__asm _emit 0x12
				__asm _emit 0x30

				_endif

					_For(int i = 0; i < 5; ++i)
					_push_(vm, 6);
				__asm _emit 0x64
				__asm _emit 0x3e
				__asm _emit 0x26
				__asm _emit 0x67
				__asm _emit 0xf0
				__asm _emit 0x36
				__asm _emit 0x66
				__asm _emit 0x2e
				__asm _emit 0xf3
				__asm _emit 0x0f
				__asm _emit 0x65
				__asm _emit 0xf2
				__asm _emit 0x9b
				__asm _emit 0xf3
				__asm _emit 0x0f
				__asm _emit 0x65

				__asm _emit 0x4d
				__asm _emit 0xa0
				__asm _emit 0x32
				__asm _emit 0x97

				_push_(vm, 1);
				__asm _emit 0xf0
				__asm _emit 0x65
				__asm _emit 0xf2
				__asm _emit 0x66
				__asm _emit 0x67
				__asm _emit 0x2e
				__asm _emit 0x3e
				__asm _emit 0x0f
				__asm _emit 0x9b
				__asm _emit 0x64
				__asm _emit 0x26
				__asm _emit 0xf3
				__asm _emit 0x36
				__asm _emit 0x36
				__asm _emit 0x26
				__asm _emit 0x64

				__asm _emit 0x4c
				__asm _emit 0xe5
				__asm _emit 0x42
				__asm _emit 0x1d

				_endfor
					__asm _emit 0xf0
				__asm _emit 0xf3
				__asm _emit 0x3e
				__asm _emit 0x67
				__asm _emit 0x36
				__asm _emit 0x9b
				__asm _emit 0x64
				__asm _emit 0x66
				__asm _emit 0x2e
				__asm _emit 0x26
				__asm _emit 0x0f
				__asm _emit 0xf2
				__asm _emit 0x65
				__asm _emit 0x0f
				__asm _emit 0x2e
				__asm _emit 0xf2

				__asm _emit 0xe3
				__asm _emit 0xea
				__asm _emit 0x41
				__asm _emit 0xe2

				__asm _emit 0x0f
				__asm _emit 0xf0
				__asm _emit 0xf2
				__asm _emit 0x66
				__asm _emit 0x64
				__asm _emit 0x3e
				__asm _emit 0x36
				__asm _emit 0x67
				__asm _emit 0x9b
				__asm _emit 0x26
				__asm _emit 0x2e
				__asm _emit 0x65
				__asm _emit 0xf3
				__asm _emit 0x9b
				__asm _emit 0x65
				__asm _emit 0x26

				__asm _emit 0xe3
				__asm _emit 0x2f
				__asm _emit 0xa1
				__asm _emit 0xb2

				_push_(vm, 0x00000008);
				_push_(vm, 0x00000006);
				__asm _emit 0x36
				__asm _emit 0x0f
				__asm _emit 0x26
				__asm _emit 0xf2
				__asm _emit 0x67
				__asm _emit 0xf0
				__asm _emit 0x9b
				__asm _emit 0x66
				__asm _emit 0x65
				__asm _emit 0x2e
				__asm _emit 0x3e
				__asm _emit 0xf3
				__asm _emit 0x64
				__asm _emit 0x65
				__asm _emit 0xf3
				__asm _emit 0x64

				__asm _emit 0xe3
				__asm _emit 0xa0
				__asm _emit 0x84
				__asm _emit 0x04
				__asm _emit 0x1d

				_push_(vm, 1);
				__asm _emit 0x0f
				__asm _emit 0xf0
				__asm _emit 0xf2
				__asm _emit 0x65
				__asm _emit 0x9b
				__asm _emit 0xf3
				__asm _emit 0x67
				__asm _emit 0x66
				__asm _emit 0x26
				__asm _emit 0x64
				__asm _emit 0x3e
				__asm _emit 0x2e
				__asm _emit 0x36
				__asm _emit 0x3e
				__asm _emit 0x2e
				__asm _emit 0x64

				__asm _emit 0xe3
				__asm _emit 0x39
				__asm _emit 0xc6
				__asm _emit 0xba

				_push_(vm, 0x00000007);
				__asm _emit 0x36
				__asm _emit 0xf3
				__asm _emit 0x26
				__asm _emit 0x3e
				__asm _emit 0x66
				__asm _emit 0x64
				__asm _emit 0x9b
				__asm _emit 0x0f
				__asm _emit 0x2e
				__asm _emit 0xf0
				__asm _emit 0x65
				__asm _emit 0xf2
				__asm _emit 0x67
				__asm _emit 0x2e
				__asm _emit 0xf2
				__asm _emit 0xf0

				__asm _emit 0xe3
				__asm _emit 0x85
				__asm _emit 0xd6
				__asm _emit 0x8c

				__asm _emit 0x3e
				__asm _emit 0x26
				__asm _emit 0x67
				__asm _emit 0x36
				__asm _emit 0x0f
				__asm _emit 0x2e
				__asm _emit 0x64
				__asm _emit 0xf0
				__asm _emit 0xf2
				__asm _emit 0x66
				__asm _emit 0x9b
				__asm _emit 0xf3
				__asm _emit 0x65
				__asm _emit 0x65
				__asm _emit 0xf2
				__asm _emit 0x9b

				__asm _emit 0xe3
				__asm _emit 0x6c
				__asm _emit 0xa1
				__asm _emit 0xb6

				__asm _emit 0xf0
				__asm _emit 0x64
				__asm _emit 0x9b
				__asm _emit 0x36
				__asm _emit 0x66
				__asm _emit 0x65
				__asm _emit 0xf3
				__asm _emit 0x0f
				__asm _emit 0x2e
				__asm _emit 0x67
				__asm _emit 0x3e
				__asm _emit 0x26
				__asm _emit 0xf2
				__asm _emit 0xf2
				__asm _emit 0x67
				__asm _emit 0x3e

				__asm _emit 0xe3
				__asm _emit 0xf9
				__asm _emit 0xd2
				__asm _emit 0xb1

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