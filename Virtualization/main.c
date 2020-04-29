#include <stdlib.h>
#include <stdio.h>

#include "VMProtect.h"

int main() {

	//_asm 
	//{
	//	mov eax, [esp]
	//};
	//vm_ptr vm;
	//DWORD op_1;
	//_asm {	
	//	
	//	lea esi, [vm] 
	//	mov esi, [esi].REG 
	//	lea eax, [esi + IP * 4] 
	//	mov ebx, op_1 
	//	mov ecx, offset ret_
	//	lea ecx, [$ + ecx] 
	//	lea esi, [vm] 
	//	mov esi, [esi].REG 
	//	lea eax, [esi + BP * 4] 
	//	mov dword ptr[eax], ecx 
	//	cmp op_1, -1 
	//	//jmp far label_1
	//	ret_: 
	//	mov eax, ebx 
	//}; 
	/*foo();*/
	DWORD res;
	BEGIN_PROTECT();
	{
		VM_MOV(0x112233, R1);
	}
	END_PROTECT(res);
	

	printf("%d\n", res);
	system("pause");
	return 0;
}