/*-----------------------------------------------------------------------------

	Реализация внутренней архитектуры виртуальной машины

-----------------------------------------------------------------------------*/


#ifndef _VM_H_
#define _VM_H_

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <memory.h>

#ifdef _WIN64 // для проверки работоспособности криптосистемы (из DLL)
#include "../Enigma/Enigma.h"
#else
#include "Enigma.h"
#endif
#include "Types.h"

#ifdef _WIN64 
#define Cip		Rip
#define GET_BYTE_CIP(ContextRecord)	(BYTE)*((DWORD*)ContextRecord->Cip)
#else
#define Cip		Eip
#define GET_BYTE_CIP(ContextRecord)	(BYTE)*((DWORD*)ContextRecord->Cip)
#endif



//---------------------------------------------------------
/*		Structures adn typedefs		*/
//---------------------------

typedef BYTE			op_type;


#pragma pack(push, 1)
typedef struct _OP_ 
{
	op_type type;
	DWORD value;
} OP;
#pragma pack(pop)

enum _types_
{
	reg_ = 0x01,
	constaddr_,
	const_,
	regaddr_
	
};

typedef void(*instrFunc)(void* pArg);

typedef struct _VM_
{
	uint32_t* CS;
	uint32_t* SS;
	BYTE* DS;

	uint32_t REG[12];

	PCRYPTOSYSTEM cs;

} VM, * VM_PTR;

//---------------------------------------------------------


//---------------------------------------------------------
/*		Consts and macroses		*/
//---------------------------

#define R0		1
#define R1		2
#define R2		3
#define	R3		4
#define	R4		5
#define R5		6
#define R6		7

#define IP		8
#define	SP		9
#define	r9		10
#define r10		11
#define COUNT_REGS r10
																								
#define FLAGS	0		/*									  |	CEF | EIF | COF | MOF |	ROF | 	?	  |	size DS |size SS (0_0 - 128 byte, 0_1 - 256 bytes, 1_0 - 512 bytes, 1_1 - 1024 bytes)
							_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |  _  |  _  |  _  |  _  |  _  | _ _ _ _ | _ _ _ _ | _ _
																
															- MOF - memory operand flag (11)
															- COF - const operand flag (12)
															- ROF - register operand flag (10)
															- EIF - Execute Instruction Flag (13)
															- CEF - Cycle Execution Flag (14)
						*/	

//---------------------------
//
//	Macroses for FLAGS register		
//

#define ROR(val, step)		((val >> step) | (val << (32 - step)))
#define ROL(val, step)		((val << step) | (val >> (32 - step)))	

#define FLAG_SIZE_SS		(vm->REG[FLAGS] & 0b11)
#define FLAG_SIZE_DS		((vm->REG[FLAGS] & 0b111100) >> 2)

#define FLAG_ROF			(ROR(vm->REG[FLAGS], 10))
#define READ_ROF			(FLAG_ROF & 0b1)
#define WRITE_ROF(bits)		(vm->REG[FLAGS] = ROL(((FLAG_ROF & 0xfffffffe) | bits), 10))

#define FLAG_MOF			(ROR(vm->REG[FLAGS], 11))
#define READ_MOF			(FLAG_MOF & 0b1)
#define WRITE_MOF(bits)		(vm->REG[FLAGS] = ROL(((FLAG_MOF & 0xfffffffe) | bits), 11))

#define FLAG_COF			(ROR(vm->REG[FLAGS], 12))
#define READ_COF			(FLAG_COF & 0b1)
#define WRITE_COF(bits)		(vm->REG[FLAGS] = ROL(((FLAG_COF & 0xfffffffe) | bits), 12))

#define FLAG_EIF			(ROR(vm->REG[FLAGS], 13))
#define READ_EIF			(FLAG_EIF & 0b1)
#define WRITE_EIF(bits)		(vm->REG[FLAGS] = ROL(((FLAG_EIF & 0xfffffffe) | bits), 13))

#define FLAG_CEF			(ROR(vm->REG[FLAGS], 14))
#define READ_CEF			(FLAG_EIF & 0b1)
#define WRITE_CEF(bits)		(vm->REG[FLAGS] = ROL(((FLAG_EIF & 0xfffffffe) | bits), 14))


//---------------------------
//
//	Consts and macroses 
//	for virtual segments
//

#define _128_B_		0
#define _256_B_		1
#define _512_B_		2
#define _1_KB_		3
#define _2_KB_		4
#define _4_KB_		5
#define _8_KB_		6
#define _16_KB_		7
#define _32_KB_		8

//------------

#define SIZE_SS				(1 << (FLAG_SIZE_SS + 7))
#define SIZE_DS				(1 << (FLAG_SIZE_DS + 7))

//------------

#define	FIRST	0
#define SECOND	4

#define WRITE_OP_DS(number, val)		*((DWORD*)((BYTE*)(vm->DS + SIZE_DS + number))) = val 

//----------------------

//#define READ_CURR_INSTR		

//----------------------

//---------------------------------------------------------


//---------------------------------------------------------
/*		Global variables		*/
//---------------------------

#define COUNT_INSTRUCTIONS	11


#define	COUNT_START_OPCODES		5
extern BYTE startOpcodes[COUNT_START_OPCODES];

/*	
									^
									|
									|
	Инструкция начала блока (pref opcode op1 op2 op3)
	op ->	0x _ _
      hight 4 bit - value start key; low 4 bit - index with rotors table |
																		 V
*/

extern BYTE valuesForRotors[16][16];
extern BYTE staticOpcodes[16][COUNT_INSTRUCTIONS];
extern BYTE staticOpTypes[16][4];

//---------------------------------------------------------


//---------------------------------------------------------
/*		Declarations functions		*/
//---------------------------


BYTE FindInOpcodeTable(BYTE opcode, BYTE index);
BYTE FindInOpTypeTable(BYTE operandByte, BYTE index);
void _push_(VM_PTR, DWORD);
void _pop_(VM_PTR vm, DWORD* dst);
uint32_t define_operand(VM_PTR vm, enum _types_ type, DWORD ex_type);
DWORD _get_secret_op_value_(VM_PTR vm, WORD value, DWORD* dst);

void _vm_init_(VM_PTR vm);
DWORD _vm_destruct_(VM_PTR vm);

//---------------------------
//
//	Instructions
//

void _vm_mov_(VM_PTR vm, OP* op1, OP* op2);
void _vm_add_(VM_PTR vm, OP* op1, OP* op2);
void _vm_sub_(VM_PTR vm, OP* op1, OP* op2);
void _vm_xor_(VM_PTR vm, OP* op1, OP* op2);
void _vm_and_(VM_PTR vm, OP* op1, OP* op2);
void _vm_or_(VM_PTR vm, OP* op1, OP* op2);
void _vm_push_(VM_PTR vm, OP* op1, OP* op2);
void _vm_pop_(VM_PTR vm, OP* op1, OP* op2);
void _vm_call_(VM_PTR vm, OP* op1, OP* op2);
void _vm_mul_(VM_PTR vm, OP* op1, OP* op2);
void _vm_div_(VM_PTR vm, OP* op1, OP* op2);
void _vm_init_cs_(VM_PTR vm, BYTE op1, BYTE op2, BYTE op3);

//---------------------------
//
//	Handle VM - instructions
//

int Handler(EXCEPTION_POINTERS* pException, VM_PTR vm);

//---------------------------------------------------------


#endif // _VM_H_