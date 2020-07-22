/*-----------------------------------------------------------------------------
	
	Модуль реализации потокового шифрования 

-----------------------------------------------------------------------------*/


#ifndef _ENIGMA_H_
#define _ENIGMA_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <time.h>
#include <memory.h>
#include "Types.h"
#include <Windows.h>

#define	TESTMODE	1


//---------------------------------------------------------
/*		Structures adn typedefs		*/
//---------------------------

typedef BYTE*	ALPH;

#pragma pack(push, 1)
typedef struct _keys_
{
	BYTE first;
	BYTE second;
	BYTE third;
} STATE, *PSTATE;
#pragma pack(pop)


typedef struct _machine_
{
	PSTATE start_state;
	ALPH alph;
	ALPH rotor_1;
	ALPH rotor_2;
	ALPH rotor_3;
	ALPH reflector;
#if TESTMODE
	PSTATE curr_state;
#endif

} MACHINE, *PMACHINE;

typedef struct _cryptosysytem_
{
	//KEYS keys;
	ALPH alph;
	PMACHINE encrypt;
	PMACHINE decrypt;

}CRYPTOSYSTEM, *PCRYPTOSYSTEM;

//---------------------------------------------------------


//---------------------------------------------------------
/*		Consts and macroses		*/
//---------------------------

#define GET_CURR_STATE(CryptoSystem, MachineType, Rotor)	\
	CryptoSystem->MachineType->curr_state->Rotor

//---------------------------------------------------------


//---------------------------------------------------------
/*		Declarations functions		*/
//---------------------------

PCRYPTOSYSTEM init_crypto();
BYTE Encrypt(PCRYPTOSYSTEM cs, BYTE value);
BYTE Decrypt(PCRYPTOSYSTEM cs, BYTE value);
void CsSetStates(PCRYPTOSYSTEM cs, PSTATE startState);
void CsSetRotors(PCRYPTOSYSTEM cs, ALPH rotor1, ALPH rotor2, ALPH rotor3);
void FreeCs(PCRYPTOSYSTEM cs);

//---------------------------------------------------------


#ifdef	__cplusplus
}
#endif

#endif //_ENIGMA_H_