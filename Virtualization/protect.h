#pragma once

#include "VMProtect.h"

#define BEGIN_PROTECT(limit_DS, limit_SS)	_BEGIN_PROTECT_(limit_DS, limit_SS)

#define VM_MOV
#define VM_OR
#define VM_AND
#define VM_XOR
#define VM_ADD
#define VM_SUB
#define VM_CALL
#define VM_PUSH
#define VM_POP
