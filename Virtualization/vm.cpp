#include "vm.h"


void _vm_init_(vm_ptr vm_)
{
	vm_->DS = (uint32_t*)malloc(512);
	vm_->SS = (uint32_t*)malloc(128);

	for (int i = 0; i < 7; ++i)
		vm_->REG[i] = 0;



}
