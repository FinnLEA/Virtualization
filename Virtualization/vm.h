#pragma once

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <memory.h>

//----------------------
#define R0		0
#define R1		1
#define R2		2
#define	R3		3
#define	R4		4
#define R5		5
#define R6		6

#define IP		7
#define	SP		8
#define	BP		9

#define FLAGS	10

//------------
#define REG_TYPE		(BYTE)1
#define CONST_TYPE		(BYTE)2

//----------------------
typedef unsigned int	uint32_t;
typedef int		DWORD;
typedef char	BYTE;
typedef BYTE	op_type;

//----------------------
typedef struct _VM_ {
	uint32_t* CS;
	uint32_t* SS;
	uint32_t* DS;

	uint32_t REG[11];


} vm_, *vm_ptr;

typedef struct _OP_ {
	op_type type;
	DWORD value;
}OP;

//----------------------
void _vm_init_(vm_ptr vm);
DWORD _vm_destruct_(vm_ptr vm);

void _vm_mov_(vm_ptr vm, BYTE op1, OP op2);
//DWORD _vm_add_(OP op1, OP op2);




