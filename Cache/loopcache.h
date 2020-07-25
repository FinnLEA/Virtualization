#ifndef _LOOPCACHE_H_
#define _LOOPCACHE_H_

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#pragma pack(push, 1)
	typedef struct _INSTR_ {
		BYTE opCount;
		BYTE opcode;
		WORD operandsTypes;
		DWORD vitrualAddr;
	}INSTR_LOOP, *PINSTR_LOOP;
#pragma pack(pop)

	//extern std::map<DWORD, PINSTR_LOOP> mapLoopCache;


	__declspec(dllexport) void RememberInstructionInCache(DWORD realAddr, PINSTR_LOOP pInstr);
	__declspec(dllexport) void RemoveInstructionInCache(DWORD realAddr);
	__declspec(dllexport) BOOL ExistInstrInCache(DWORD realAddr);
	__declspec(dllexport) BYTE GetOpcodeFromCache(DWORD realAddr);
	__declspec(dllexport) BYTE GetOperandTypeFromCache(DWORD realAddr, BYTE countOp);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_LOOPCACHE_H_
