#pragma once

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <memory.h>

//----------------------

//----------------------
typedef unsigned int	uint32_t;
typedef int		DWORD;
typedef char	BYTE;
typedef BYTE	op_type;

// extern DWORD _init_ops_(DWORD op1, DWORD op2, DWORD* vR9_reg);

typedef struct _OP_ 
{
	op_type type;
	DWORD value;
} OP;

typedef struct _operand_type_
{
	op_type op;
} CURROPTYPE, *PCURROPTYPE;

enum _types_
{
	reg_,
	imm_,
	comst_
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
																				 // пока аналогично, но можно и больше(4 бита отведено (не больше 32 Кбайт)
#define FLAGS	0		/*									|COF | MOF |	 | 	?	   |	DS  |size SS (0_0 - 128 byte, 0_1 - 256 bytes, 1_0 - 512 bytes, 1_1 - 1024 bytes)
							_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |_ _ | _ _ | _ _ | _ _ _ _ |_ _ _ _ | _ _
																
															- COF - conts operand flag (12 - 13)
															- MOF - memory operand flag (14 - 15)
						*/

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
#define FLAG_SIZE_SS		(vm->REG[FLAGS] & 0b11)
#define FLAG_SIZE_DS		((vm->REG[FLAGS] & 0b111100) >> 2)


#define SIZE_SS				(1 << (FLAG_SIZE_SS + 7))
#define SIZE_DS				(1 << (FLAG_SIZE_DS + 7))

//----------------------
typedef struct _VM_ 
{
	uint32_t* CS;
	uint32_t* SS;
	uint32_t* DS;

	uint32_t REG[11];


} vm_, *vm_ptr;


#define _MOV_OPCODE_		\
	{ 0x00, 0x01, 0x01, 0x01 }, \
	{ 0x00, 0x01, 0x01, 0x02 }

#define _ADD_OPCODE_		\
	{ 0x00, 0x02, 0x01, 0x01 }, \
	{ 0x00, 0x02, 0x01, 0x02 }

#define OPCODE_TABLE		\
	{\
		_MOV_OPCODE_, \
		_ADD_OPCODE_ \
	}
		//{ 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 }, \ 
		//{ 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01 }, \
		//{ 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02 }, \
		//{ 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03 }, \
		//{ 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01 }, \
		//{ 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x02 }, \
		//{ 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03 }, \
		//{ 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01 }, \
		//{ 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02 }, \
		//{ 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03 }, \
		//{ 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01 }, \
		//{ 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x02 }, \
		//{ 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03 } \
	//}


#define INTSR_COUNT	43
//static uint32_t** opcode_table;
static BYTE table[4][4] = OPCODE_TABLE;
//----------------------
static void _generate_opcode_table_();
void _push_(vm_ptr vm, DWORD value);
void _pop_(vm_ptr vm, DWORD* dst);
uint32_t define_operand(vm_ptr vm, enum _types_ type, DWORD ex_type);


void _vm_init_(vm_ptr vm);
DWORD _vm_destruct_(vm_ptr vm);

void _vm_mov_(vm_ptr vm, BYTE op1, OP op2);
void _vm_add_(vm_ptr vm, OP op1, OP op2);
void _vm_sub_(vm_ptr vm, OP op1, OP op2);
void _vm_xor_(vm_ptr vm, OP op1, OP op2);
void _vm_and_(vm_ptr vm, OP op1, OP op2);
void _vm_or_(vm_ptr vm, OP op1, OP op2);
void _vm_push_(vm_ptr vm, OP op);
void _vm_pop_(vm_ptr vm, OP op);
void _vm_call_(vm_ptr vm, DWORD addr);

void foo();

