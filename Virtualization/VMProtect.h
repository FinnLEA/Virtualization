#pragma once
#ifndef  _VM_PROTECT_
#define _VM_PROTECT_


#include "vm.h"

#ifdef _WIN64
#define Cip		Rip
#define GET_BYTE_CIP(ContextRecord)	(BYTE)*((DWORD*)ContextRecord->Cip)
#else
#define Cip		Eip
#define GET_BYTE_CIP(ContextRecord)	(BYTE)*((DWORD*)ContextRecord->Cip)
#endif

//----------------------
#define r0		/*\
	define_operand(vm, reg_, R0)*/
#define r1		/*\
	define_operand(vm, reg_, R1)*/
#define r2		/*\
	define_operand(vm, reg_, R2)*/
#define r3		/*\
	define_operand(vm, reg_, R3)*/
#define r4		/*\
	define_operand(vm, reg_, R4)*/
#define r5		/*\
	define_operand(vm, reg_, R5)*/
#define r6		/*\
	define_operand(vm, reg_, R6)*/

/* ex. r1
	v_sp <- type						ex.: sp-> 0x00000001 -> r1 // убрал
	r9 <- [ (type|num_reg)| мусор ]		ex.: r9 = 0x000012cd -> r1
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
	define_operand(vm, constaddr_, addr) //в FLAGS бит ставить

/* ex. [0x11223344]
	v_sp <- (type + addr)					ex.: sp-> 0x11223346 
	r9 <- [ (type) ] | first_byte addr		ex.: r9 = 0x00000244
			1 byte   |	1 byte
*/		    			

#define CONST(val)	\
	define_operand(vm, const_, val)

/* ex. 0x11223344
	v_sp <- value							ex.: sp-> 0x11223344
	r9 <- [ (type) ] | мусор				ex.: r9 = 0x000003ff
			1 byte   | 1 byte
*/

// При двух операнах код первого операнда сдвигается пр.: vm_mov r1, imm(0x11223344) -> r9 = 0x12cd0244

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
#define _BEGIN_PROTECT_(limit_DS, limit_SS) \
	vm_ptr vm = (vm_ptr)malloc(sizeof(vm_)); \
	vm->REG[FLAGS] = 0; \
	_DEFINE_FLAG_SIZE_SS_(limit_SS) \
	_DEFINE_FLAG_SIZE_DS_(limit_DS) \
	_vm_init_(vm); \
	OP* op1 = (OP*)malloc(sizeof(OP)); \
	OP* op2 = (OP*)malloc(sizeof(OP)); \
	vm->cs = init_crypto();



#define END_PROTECT(res)	\
	res = _vm_destruct_(vm);

	

#define GET_OP_TYPE (op) \
	((op >> 8) & 0x0f)

//DWORD init_opcode(vm_ptr vm, PCRYPTOSYSTEM cs)
//{
//	DWORD opcode = 0;
//	BYTE tmp = 0; 
//
//	// op 2
//	tmp = Encrypt(cs, ((vm->REG[r9] >> 8) & 0b1111)); 
//	tmp = (tmp << 4) | Encrypt(cs, (((vm->REG[r9] >> 8) & 0xF0) >> 4)); 
//	opcode |= tmp;
//
//	//op 1
//	tmp = Encrypt(cs, ((vm->REG[r9] >> 24) & 0b1111));
//	tmp = (tmp << 4) | Encrypt(cs, (((vm->REG[r9] >> 24) & 0xF0) >> 4));
//	opcode <<= 8;
//	opcode |= tmp;
//
//	// instr
//	tmp = Encrypt(cs, 0x01);
//	//tmp = (tmp << 4) | Encrypt(cs, (((vm->REG[r9] >> 8) & 0xF0) >> 4));
//	opcode <<= 8;
//	opcode |= tmp;
//
//	return opcode;
//}

// int Handler(EXCEPTION_POINTERS *pException) {
// 	if (pException->ExceptionRecord->ExceptionCode == STATUS_ILLEGAL_INSTRUCTION) {
// 		pException->ContextRecord->Eip += 0x06;
// 		//pException->ContextRecord->Ecx = 1;
// 		return EXCEPTION_CONTINUE_EXECUTION;
// 	}
// 	else {
// 		return EXCEPTION_EXECUTE_HANDLER;
// 	}
// }

// void SetInstruction() {
// 	__try {
// 		DD;
// 	}
// 	__except (Handler(GetExceptionInformation())) {
// 		return 1;
// 	}
// }

#define VM_MOV(op_1, op_2)	\
	op_1; op_2; \
	init_opcode(vm, cs); \
	_init_ops_(vm, op1, op2); \
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


