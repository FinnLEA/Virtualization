//#include "Enigma.h"
#include <Windows.h>
#include "Enigma.h"

//void __declspec(dllexport) func() {
//	MessageBox(NULL, "test", "", MB_OK);
//}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		
		break;
	}

	return TRUE;
}
