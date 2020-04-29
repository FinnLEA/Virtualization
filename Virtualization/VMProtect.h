#pragma once
#ifndef  _VM_PROTECT_
#define _VM_PROTECT_


#include "vm.h"

//----------------------
#define r0		\
	define_operand(vm, reg_, R0)
#define r1		\
	define_operand(vm, reg_, R1)
#define r2		\
	define_operand(vm, reg_, R2)
#define r3		\
	define_operand(vm, reg_, R3)
#define r4		\
	define_operand(vm, reg_, R4)
#define r5		\
	define_operand(vm, reg_, R5)
#define r6		\
	define_operand(vm, reg_, R6)

#define IMM(addr)	\
	define_operand(vm, imm_, addr ^ 0x10) //в FLAGS бит ставить

//-----------------
#define _128_BYTE	0x80
#define _256_BYTE	0x100
#define _512_BYTE	0x200
#define _1_KBYTE	0x400
#define _2_KBYTE	0x800
#define _4_KBYTE	0x1000
#define _8_KBYTE	0x2000
#define _16_KBYTE	0x4000
#define _32_KBYTE	0x8000

#define __MOV_REG_REG__		[0][0]

#define _CURR_INSTRUCTION_	vm->REG[IP]

//----------------------
#define _DEFINE_FLAG_SIZE_SS_(limit_SS)		\
	if(!(limit_SS % 128) && (limit_SS <= _1_KBYTE)) \
		switch(limit_SS / 128) \
		{ \
			case 1: \
				vm->REG[FLAGS] |= _128_B_; \
				break; \
			case 2: \
				vm->REG[FLAGS] |= _256_B_; \
				break; \
			case 4: \
				vm->REG[FLAGS] |= _512_B_; \
				break; \
			case 8: \
				vm->REG[FLAGS] |= _1_KB_; \
				break; \
			default: \
				vm->REG[FLAGS] |= _256_B_; \
				break; \
		} \
	else \
		vm->REG[FLAGS] |= _256_B_;


#define _DEFINE_FLAG_SIZE_DS_(limit_DS)		\
	if(!(limit_DS % 128) && (limit_DS <= _32_KBYTE)) \
		switch(limit_DS / 128) \
		{ \
			case 1: \
				vm->REG[FLAGS] |= (_128_B_ << 2); \
				break; \
			case 2: \
				vm->REG[FLAGS] |= (_256_B_ << 2); \
				break; \
			case 4: \
				vm->REG[FLAGS] |= (_512_B_ << 2); \
				break; \
			case 8: \
				vm->REG[FLAGS] |= (_1_KB_ << 2); \
				break; \
			case 16: \
				vm->REG[FLAGS] |= (_2_KB_ << 2); \
				break; \
			case 32: \
				vm->REG[FLAGS] |= (_4_KB_ << 2); \
				break; \
			case 64: \
				vm->REG[FLAGS] |= (_8_KB_ << 2); \
				break; \
			case 128: \
				vm->REG[FLAGS] |= (_16_KB_ << 2); \
				break; \
			case 256: \
				vm->REG[FLAGS] |= (_32_KB_ << 2); \
				break; \
			default: \
				vm->REG[FLAGS] |= (_512_B_ << 2); \
				break; \
		} \
	else \
		vm->REG[FLAGS] |= (_512_B_ << 2);

//----------------------
#define BEGIN_PROTECT(limit_DS, limit_SS) \
	vm_ptr vm = (vm_ptr)malloc(sizeof(vm_)); \
	vm->REG[FLAGS] = 0; \
	_DEFINE_FLAG_SIZE_SS_(limit_SS) \
	_DEFINE_FLAG_SIZE_DS_(limit_DS) \
	_vm_init_(vm); \
	PCURROPTYPE type = (PCURROPTYPE)malloc(sizeof(CURROPTYPE));
	
	

#define END_PROTECT(res)	\
	_asm{\
		mov eax, 1 \
	} \
	res = _vm_destruct_(vm);


#define VM_MOV(op_1, op_2)	\
	op_1; op_2; \
	OP* op1 = (OP*)malloc(sizeof(OP)); \
	OP* op2 = (OP*)malloc(sizeof(OP)); \
	op1 = _init_ops_(op1, op2, &vm->REG[r9]); \
	vm->CS[_CURR_INSTRUCTION_] = table __MOV_REG_REG__; \
	_CURR_INSTRUCTION_ += 4;

#endif // ! _VM_PROTECT_


