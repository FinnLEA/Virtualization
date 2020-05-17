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
	PSTATE curr_state;
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
	PSTATE curr_state;
#endif

} DECRYPT, *PDECRYPT;

typedef struct _cryptosysytem_
{
	//KEYS keys;
	PENCRYPT enctypt;
	PDECRYPT decrypt;

}CRYPTOSYSTEM, *PCRYPTOSYSTEM;

//----------------------

PCRYPTOSYSTEM init_crypto();

