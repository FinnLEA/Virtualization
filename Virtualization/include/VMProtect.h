/*-----------------------------------------------------------------------------

	 Спец. макросы виртуальной машины

-----------------------------------------------------------------------------*/


#ifndef  _VM_PROTECT_
#define _VM_PROTECT_


#include "vm.h"


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
	VM_PTR vm = (VM_PTR)malloc(sizeof(VM)); \
	vm->REG[FLAGS] = 0; \
	_DEFINE_FLAG_SIZE_SS_(limit_SS) \
	_DEFINE_FLAG_SIZE_DS_(limit_DS) \
	_vm_init_(vm); \
	OP* op1 = (OP*)malloc(sizeof(OP)); \
	OP* op2 = (OP*)malloc(sizeof(OP)); \
	vm->cs = init_crypto();



#define END_PROTECT(res)	\
	res = _vm_destruct_(vm);



//#define VM_MOV(op_1, op_2)	\
//	op_1; op_2; \
//	init_opcode(vm, cs); \
//	_init_ops_(vm, op1, op2); \
//	_vm_mov_(vm, op1, op2); \
//	_CURR_INSTRUCTION_ += 4; \
//	if((READ_MOF) || (READ_COF) || (READ_ROF)) { \
//		WRITE_MOF(0); \
//		WRITE_COF(0); \
//		WRITE_ROF(0); \
//	} 
//	//free(op1); \
//	//free(op2); 
//
//
//#define VM_ADD(op_1, op_2)	\
//	op_1; op_2; \
//	_init_ops_(vm, op1, op2); \
//	_vm_add_(vm, op1, op2); \
//	_CURR_INSTRUCTION_ += 4; \
//	if(READ_MOF || READ_COF || READ_ROF) { \
//		WRITE_MOF(0); \
//		WRITE_COF(0); \
//		WRITE_ROF(0); \
//	} 
//	//free(op1); \
//	//free(op2); 
//
//
//#define VM_SUB(op_1, op_2)	\
//	op_1; op_2; \
//	_init_ops_(vm, op1, op2); \
//	_vm_sub_(vm, op1, op2); \
//	_CURR_INSTRUCTION_ += 4; \
//	if(READ_MOF || READ_COF || READ_ROF) { \
//		WRITE_MOF(0); \
//		WRITE_COF(0); \
//		WRITE_ROF(0); \
//	} 
//	//free(op1); \
//	//free(op2); 
//
//
//#define VM_XOR(op_1, op_2)	\
//	op_1; op_2; \
//	_init_ops_(vm, op1, op2); \
//	_vm_xor_(vm, op1, op2); \
//	_CURR_INSTRUCTION_ += 4; \
//	if(READ_MOF || READ_COF || READ_ROF) { \
//		WRITE_MOF(0); \
//		WRITE_COF(0); \
//		WRITE_ROF(0); \
//	}
//	//free(op1); \
//	//free(op2); 
//
//
//#define VM_AND(op_1, op_2)	\
//	op_1; op_2; \
//	_init_ops_(vm, op1, op2); \
//	_vm_and_(vm, op1, op2); \
//	_CURR_INSTRUCTION_ += 4; \
//	if(READ_MOF || READ_COF || READ_ROF) { \
//		WRITE_MOF(0); \
//		WRITE_COF(0); \
//		WRITE_ROF(0); \
//	}
//	//free(op1); \
//	//free(op2); 
//
//
//#define VM_OR(op_1, op_2)	\
//	op_1; op_2; \
//	_init_ops_(vm, op1, op2); \
//	_vm_or_(vm, op1, op2); \
//	_CURR_INSTRUCTION_ += 4; \
//	if(READ_MOF || READ_COF || READ_ROF) { \
//		WRITE_MOF(0); \
//		WRITE_COF(0); \
//		WRITE_ROF(0); \
//	} 
//	//free(op1); \
//	//free(op2); 

#endif // ! _VM_PROTECT_


