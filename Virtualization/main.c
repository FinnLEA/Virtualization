#include <stdlib.h>
#include <stdio.h>

#include "VMProtect.h"

int main() {

	//_asm 
	//{
	//	mov eax, [esp]
	//};
	//vm_ptr vm;
	//DWORD op_1;
	//_asm {	
	//	
	//	lea esi, [vm] 
	//	mov esi, [esi].REG 
	//	lea eax, [esi + IP * 4] 
	//	mov ebx, op_1 
	//	mov ecx, offset ret_
	//	lea ecx, [$ + ecx] 
	//	lea esi, [vm] 
	//	mov esi, [esi].REG 
	//	lea eax, [esi + BP * 4] 
	//	mov dword ptr[eax], ecx 
	//	cmp op_1, -1 
	//	//jmp far label_1
	//	ret_: 
	//	mov eax, ebx 
	//}; 
	/*foo();*/
	DWORD res;
	//vm_ptr vm = (vm_ptr)malloc(sizeof(vm_)); 
	//vm->REG[FLAGS] = 0;
	//	//_DEFINE_FLAG_SIZE_SS_(128) 
	//if (!(256 % 128)) 
	//	switch (256 / 128) 
	//	{ 
	//	case 1: 
	//		vm->REG[FLAGS] |= _128_; 
	//		break; 
	//	case 2: 
	//		vm->REG[FLAGS] |= _256_; 
	//		break; 
	//	case 4: 
	//		vm->REG[FLAGS] |= _512_; 
	//		break; 
	//	case 8: 
	//		vm->REG[FLAGS] |= _1024_;
	//		break; 
	//	default: 
	//		vm->REG[FLAGS] |= _256_; 
	//		break; 
	//	} 
	//else 
	//	vm->REG[FLAGS] |= _256_;
	//	_vm_init_(vm); 
	//	PCURROPTYPE type = (PCURROPTYPE)malloc(sizeof(CURROPTYPE));
	BEGIN_PROTECT(_1_KBYTE, _256_BYTE);
	{
		DWORD res1;
		_push_(vm, 2);
		_pop_(vm, &res1);
		
		//define_operand(vm, reg_, R0);
		//_init_ops_(0x112233, define_operand(vm, reg_, R0), vm->REG[r9]);
		VM_MOV(r0, CONST(0xffffffff));
		
		//_init_ops_(0x112233, r0, &vm->REG[BP]); \
		//	vm->CS[_CURR_INSTRUCTION_] = table __MOV_REG_REG__; \
		//	OP op2; \
		//	op2.type = CONST_TYPE; \
		//	op2.value = op_2;	\
		//	_vm_mov_(vm, op_1, op2); \
		//	_CURR_INSTRUCTION_ += 4;
		res = 0;
	}
	END_PROTECT(res);

	printf("%d\n", res);
	system("pause");
	return 0;
}