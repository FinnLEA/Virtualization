/*-----------------------------------------------------------------------------

	Интерфейсные определения инструкций  

-----------------------------------------------------------------------------*/


#ifndef _PROTECT_H_
#define _PROTECT_H_

#include "VMProtect.h"

//#define BEGIN_PROTECT(limit_DS, limit_SS)	_BEGIN_PROTECT_(limit_DS, limit_SS)


//---------------------------
//	Mnemonics
//

#define VM_MOV
#define vm_mov
#define VM_OR
#define vm_or
#define VM_AND
#define vm_and
#define VM_XOR
#define vm_xor
#define VM_ADD
#define vm_add
#define VM_SUB
#define vm_sub
#define VM_CALL
#define vm_call
#define VM_PUSH
#define vm_push
#define VM_POP
#define vm_pop
#define VM_MUL
#define vm_mul
#define VM_DIV
#define vm_div

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
