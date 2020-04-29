#pragma once
#ifndef  _VM_PROTECT_
#define _VM_PROTECT_


#include "vm.h"

//----------------------
#define __MOV_REG_REG__		[0][0]

#define _CURR_INSTRUCTION_	vm->REG[IP]

//----------------------
#define BEGIN_PROTECT() \
	vm_ptr vm = (vm_ptr)malloc(sizeof(vm_));	\
	_vm_init_(vm);							
	
	

#define END_PROTECT(res)	\
	_asm{\
		mov eax, 1 \
	} \
	res = _vm_destruct_(vm);


#define VM_MOV(op_1, op_2)	\
	_init_ops_(op_1, op_2, &vm->REG[BP]); \
	vm->CS[_CURR_INSTRUCTION_] = table __MOV_REG_REG__; \
	OP op2; \
	op2.type = CONST_TYPE; \
	op2.value =  op_2;	\
	_vm_mov_(vm, op_1, op2); \
	_CURR_INSTRUCTION_ += 4;

#endif // ! _VM_PROTECT_


