
#include "pch.h"


std::map<DWORD, PINSTR_LOOP> mapLoopCache;

void RememberInstructionInCache(DWORD realAddr, PINSTR_LOOP pInstr) {
	mapLoopCache.insert(std::make_pair(realAddr, pInstr));
}

BOOL ExistInstrInCache(DWORD realAddr) {
	if (mapLoopCache[realAddr]) {
		return true;
	}

	return false;
}

BYTE GetOpcodeFromCache(DWORD realAddr) {
	if (ExistInstrInCache(realAddr)) {
		return mapLoopCache[realAddr]->opcode;
	}
	// except
	return 0;
}

BYTE GetOperandTypeFromCache(DWORD realAddr, BYTE countOp) {
	if (ExistInstrInCache(realAddr)) {
		if (mapLoopCache[realAddr]->opCount == 2) {
			switch (countOp) {
			case 1:
				return (mapLoopCache[realAddr]->operandsTypes & 0xff00);

			case 2:
				return (mapLoopCache[realAddr]->operandsTypes & 0x00ff);

			default:
				// except
				break;
			}

		}
		else {
			return (mapLoopCache[realAddr]->operandsTypes & 0x00ff);
		}
	}
	
	// except
	return 0;
}


