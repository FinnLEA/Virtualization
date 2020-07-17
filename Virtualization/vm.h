#pragma once

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <memory.h>

#ifdef _WIN64
#include "../Enigma/Enigma.h"
#else
#include "Enigma.h"
#endif
#include "Types.h"



//----------------------
typedef BYTE			op_type;

//----------------------
//
//#pragma pack(push, 1)
//typedef struct _keys_
//{
//	BYTE first;
//	BYTE second;
//	BYTE third;
//} KEYS, *PKEYS;
//#pragma pack(pop)
//
//static KEYS keys;
////----------------------
//
//static void genric_keys();

//----------------------

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

//----------------------

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
																								// ���� ����������, �� ����� � ������(4 ���� �������� (�� ������ 32 �����)
#define FLAGS	0		/*										| EIF | COF | MOF |	ROF | 	?	  |	DS     |size SS (0_0 - 128 byte, 0_1 - 256 bytes, 1_0 - 512 bytes, 1_1 - 1024 bytes)
							_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _	|  _  |  _  |  _  |  _  | _ _ _ _ |_ _ _ _ | _ _
																
															- MOF - memory operand flag (11)
															- COF - const operand flag (12)
															- ROF - register operand flag (10)
															- EIF - Execute Instruction Flag (13)
						*/

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


//----------------------
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



typedef struct _VM_ 
{
	uint32_t* CS;
	uint32_t* SS;
	BYTE* DS;

	uint32_t REG[11];

	PCRYPTOSYSTEM cs;

} vm_, *vm_ptr;


#define _MOV_OPCODE_		\
	{ 0x00, 0x01, 0x00, 0x00 }

#define _ADD_OPCODE_		\
	{ 0x00, 0x02, 0x00, 0x00 }

#define OPCODE_TABLE		\
	{\
		_MOV_OPCODE_, \
		_ADD_OPCODE_ \
	}


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




#define INTSR_COUNT	43
//static uint32_t** opcode_table;
static BYTE table[4][4] = OPCODE_TABLE;
//----------------------
void _generate_opcode_table_();
void _push_(vm_ptr, DWORD);
void _pop_(vm_ptr vm, DWORD* dst);
uint32_t define_operand(vm_ptr vm, enum _types_ type, DWORD ex_type);
DWORD _get_secret_op_value_(vm_ptr vm, WORD value, DWORD* dst);

void _vm_init_(vm_ptr vm);
DWORD _vm_destruct_(vm_ptr vm);

void _vm_mov_(vm_ptr vm, OP* op1, OP* op2);
void _vm_add_(vm_ptr vm, OP* op1, OP* op2);
void _vm_sub_(vm_ptr vm, OP* op1, OP* op2);
void _vm_xor_(vm_ptr vm, OP* op1, OP* op2);
void _vm_and_(vm_ptr vm, OP* op1, OP* op2);
void _vm_or_(vm_ptr vm, OP* op1, OP* op2);
void _vm_push_(vm_ptr vm, OP* op1, OP* op2);
void _vm_pop_(vm_ptr vm, OP* op1, OP* op2);
void _vm_call_(vm_ptr vm, OP* op);
void _vm_mul_(vm_ptr vm, OP* op1, OP* op2);
void _vm_div_(vm_ptr vm, OP* op1, OP* op2);
void _vm_init_cs_(vm_ptr vm, BYTE op1, BYTE op2, BYTE op3);

