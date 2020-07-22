/*-----------------------------------------------------------------------------

	������������ ����������� ����������  

-----------------------------------------------------------------------------*/


#ifndef _PROTECT_H_
#define _PROTECT_H_

#include "VMProtect.h"

//#define BEGIN_PROTECT(limit_DS, limit_SS)	_BEGIN_PROTECT_(limit_DS, limit_SS)


//---------------------------
//	Mnemonics
//

#define VM_MOV
#define VM_OR
#define VM_AND
#define VM_XOR
#define VM_ADD
#define VM_SUB
#define VM_CALL
#define VM_PUSH
#define VM_POP
#define VM_MUL
#define VM_DIV

//---------------------------
//	Registers
//

#define r0		
#define r1		
#define r2		
#define r3		
#define r4		
#define r5		
#define r6		




#define _While(expression)	while(expression) {
#define _endw		}
#define _For(expression)	for(expression) {
#define _endfor		}

#define _If(expression)		if(expression){
#define _Elif(expression)	else if(expression){
#define _Else		else{
#define _endif		}
#define _ENDIF		}



#define STATIC_CODE
#define static_code
#define ENDS
#define ends

#define BEGIN_PROTECT

#define _128_BYTE	0x80
#define _256_BYTE	0x100
#define _512_BYTE	0x200
#define _1_KBYTE	0x400
#define _2_KBYTE	0x800
#define _4_KBYTE	0x1000
#define _8_KBYTE	0x2000
#define _16_KBYTE	0x4000
#define _32_KBYTE	0x8000

#endif //_PROTECT_H_
