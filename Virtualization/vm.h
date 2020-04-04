#ifndef _vm_h_
#define _vm_h_

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

//----------------------
typedef unsigned int	uint32_t;
typedef uint32_t		DWORD;
typedef unsigned char	BYTE;

//----------------------
typedef struct _VM_ {
	uint32_t* CS;
	uint32_t* SS;
	uint32_t* DS;

	uint32_t REG[11];


} vm, *vm_ptr;

//----------------------
void _vm_init_(vm_ptr vm_);
	

#endif // _vm_h


