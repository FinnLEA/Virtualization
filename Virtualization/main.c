#include <stdlib.h>
#include <stdio.h>

#include "VMProtect.h"

int main() {
	DWORD res;
	BEGIN_PROTECT();
	{
		VM_MOV(R0, 6);
	}
	END_PROTECT(res);

	printf("%d\n", res);
	system("pause");
	return 0;
}