#pragma once

#include <time.h>
#include <memory.h>
#include "Types.h"

#define	TESTMODE	1

//----------------------

typedef BYTE*	ALPH;

//----------------------


#pragma pack(push, 1)
typedef struct _keys_
{
	BYTE first;
	BYTE second;
	BYTE third;
} STATE, *PSTATE;
#pragma pack(pop)

typedef struct _encrypt_
{
	PSTATE start_state;
	ALPH alph;
	ALPH rotor_1;
	ALPH rotor_2;
	ALPH rotor_3;
	ALPH reflector;
#if TESTMODE
	STATE curr_state;
#endif

} ENCRYPT, *PENCRYPT;

typedef struct _decrypt_
{
	PSTATE start_state;
	ALPH alph;
	ALPH rotor_1;
	ALPH rotor_2;
	ALPH rotor_3;
	ALPH reflector;
#if TESTMODE
	STATE curr_state;
#endif

} DECRYPT, *PDECRYPT;

typedef struct _cryptosysytem_
{
	//KEYS keys;
	ALPH alph;
	PENCRYPT encrypt;
	PDECRYPT decrypt;

}CRYPTOSYSTEM, *PCRYPTOSYSTEM;

//----------------------

#define GET_CURR_STATE(CryptoSystem, MachineType, Rotor)	\
	CryptoSystem->MachineType->curr_state.Rotor

//----------------------

PCRYPTOSYSTEM init_crypto();
BYTE Encrypt(PCRYPTOSYSTEM cs, BYTE value);
BYTE Decrypt(PCRYPTOSYSTEM cs, BYTE value);
