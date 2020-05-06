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

/* ex. r1
	v_sp <- type						ex.: sp-> 0x00000001 -> r1
	r9 <- [ (type|num_reg)| мусор ]	ex.: r9 = 0x000012cd -> r1
		     4бит| 4бит	  | 1байт
*/

#define W_IMM(addr)	\
	//WRITE_DIMM(1) \
	// IMM(addr)

#define B_IMM(addr) \
	//WRITE_DIMM(1) \
	// IMM(addr)


#define D_IMM(addr)	\
	//WRITE_DIMM(1) \
	// IMM(addr)

#define IMM(addr)	\
	define_operand(vm, imm_, addr) //в FLAGS бит ставить

/* ex. [0x11223344]
	v_sp <- (type + addr)					ex.: sp-> 0x11223346 
	r9 <- [ (type) ] | first_byte addr	ex.: r9 = 0x00000244
			1 byte   |	1 byte
*/		    			

#define CONST(val)	\
	define_operand(vm, const_, val)

/* ex. 0x11223344
	v_sp <- value							ex.: sp-> 0x11223344
	r9 <- [ (type) ] | мусор			ex.: r9 = 0x000003ff
			1 byte   | 1 byte
*/

// При двух операнах код первого операнда сдвигается пр.: vm_mov r1, imm(0x11223344) -> r9 = 0x12cd03ff

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
	OP* op1 = (OP*)malloc(sizeof(OP)); \
	OP* op2 = (OP*)malloc(sizeof(OP)); 
	//PCURROPTYPE type = (PCURROPTYPE)malloc(sizeof(CURROPTYPE));
	
	

#define END_PROTECT(res)	\
	_asm{\
		mov eax, 1 \
	} \
	res = _vm_destruct_(vm);


#define VM_MOV(op_1, op_2)	\
	op_1; op_2; \
	_init_ops_(vm, op1, op2); \
	vm->CS[_CURR_INSTRUCTION_] = table __MOV_REG_REG__; \
	_vm_mov_(vm, op1, op2); \
	_CURR_INSTRUCTION_ += 4; \
	if((READ_MOF) || (READ_COF) || (READ_ROF)) { \
		WRITE_MOF(0); \
		WRITE_COF(0); \
		WRITE_ROF(0); \
	} 
	//free(op1); \
	//free(op2); 


#define VM_ADD(op_1, op_2)	\
	op_1; op_2; \
	_init_ops_(vm, op1, op2); \
	_vm_add_(vm, op1, op2); \
	_CURR_INSTRUCTION_ += 4; \
	if(READ_MOF || READ_COF || READ_ROF) { \
		WRITE_MOF(0); \
		WRITE_COF(0); \
		WRITE_ROF(0); \
	} 
	//free(op1); \
	//free(op2); 


#define VM_SUB(op_1, op_2)	\
	op_1; op_2; \
	_init_ops_(vm, op1, op2); \
	_vm_sub_(vm, op1, op2); \
	_CURR_INSTRUCTION_ += 4; \
	if(READ_MOF || READ_COF || READ_ROF) { \
		WRITE_MOF(0); \
		WRITE_COF(0); \
		WRITE_ROF(0); \
	} 
	//free(op1); \
	//free(op2); 


#define VM_XOR(op_1, op_2)	\
	op_1; op_2; \
	_init_ops_(vm, op1, op2); \
	_vm_xor_(vm, op1, op2); \
	_CURR_INSTRUCTION_ += 4; \
	if(READ_MOF || READ_COF || READ_ROF) { \
		WRITE_MOF(0); \
		WRITE_COF(0); \
		WRITE_ROF(0); \
	}
	//free(op1); \
	//free(op2); 


#define VM_AND(op_1, op_2)	\
	op_1; op_2; \
	_init_ops_(vm, op1, op2); \
	_vm_and_(vm, op1, op2); \
	_CURR_INSTRUCTION_ += 4; \
	if(READ_MOF || READ_COF || READ_ROF) { \
		WRITE_MOF(0); \
		WRITE_COF(0); \
		WRITE_ROF(0); \
	}
	//free(op1); \
	//free(op2); 


#define VM_OR(op_1, op_2)	\
	op_1; op_2; \
	_init_ops_(vm, op1, op2); \
	_vm_or_(vm, op1, op2); \
	_CURR_INSTRUCTION_ += 4; \
	if(READ_MOF || READ_COF || READ_ROF) { \
		WRITE_MOF(0); \
		WRITE_COF(0); \
		WRITE_ROF(0); \
	} 
	//free(op1); \
	//free(op2); 

#endif // ! _VM_PROTECT_


