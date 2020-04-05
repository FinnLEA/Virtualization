#pragma once
#ifndef  _VM_PROTECT_
#define _VM_PROTECT_


#include "vm.h"

//----------------------


//----------------------
#define BEGIN_PROTECT() \
	vm_ptr vm = (vm_ptr)malloc(sizeof(vm_));	\
	_vm_init_(vm); 
	

#define END_PROTECT(res)	\
	_asm{} \
	res = _vm_destruct_(vm);

#define VM_MOV(op_1, op_2)	\
	OP op2; \
	op2.type = CONST_TYPE; \
	op2.value =  op_2;	\
	_vm_mov_(vm, op_1, op2);
	

#endif // ! _VM_PROTECT_


