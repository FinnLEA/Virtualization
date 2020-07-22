/*-----------------------------------------------------------------------------

	Точка входа Enigma.dll

-----------------------------------------------------------------------------*/


#include <Windows.h>
#include "Enigma.h"



BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		
		break;
	}

	return TRUE;
}
