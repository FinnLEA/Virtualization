/*-----------------------------------------------------------------------------

	Модуль реализации потокового шифрования для DLL

-----------------------------------------------------------------------------*/


#include "Enigma.h"


//---------------------------------------------------------
/*		Definitions functions		*/
//---------------------------

static BYTE mod16(BYTE value) {
	return ((value % 16) + 16) % 16;
}

__declspec(dllexport)
void CsSetStates(PCRYPTOSYSTEM cs, PSTATE startState) {
	//cs->decrypt->start_state = cs->encrypt->start_state = (PSTATE)malloc(sizeof(STATE));

	cs->decrypt->start_state = cs->encrypt->start_state = startState;

	cs->encrypt->curr_state->first		= cs->decrypt->curr_state->first	= startState->first;
	cs->encrypt->curr_state->second		= cs->decrypt->curr_state->second	= startState->second;
	cs->encrypt->curr_state->third		= cs->decrypt->curr_state->third	= startState->third;
}

__declspec(dllexport)
void CsSetRotors(PCRYPTOSYSTEM cs, ALPH rotor1, ALPH rotor2, ALPH rotor3) {
	cs->decrypt->rotor_1 = cs->encrypt->rotor_1 = rotor1;
	cs->decrypt->rotor_2 = cs->encrypt->rotor_2 = rotor2;
	cs->decrypt->rotor_3 = cs->encrypt->rotor_3 = rotor3;
}

static void generic_start_state(PCRYPTOSYSTEM cs)
{
	
	PSTATE start_state = (PSTATE)malloc(sizeof(STATE));

	start_state->first = cs->alph[0] + rand() % cs->alph[15];
	start_state->second = cs->alph[0] + rand() % cs->alph[15];
	while (start_state->first == start_state->second)
		start_state->second = cs->alph[0] + rand() % cs->alph[15];

	start_state->third = cs->alph[0] + rand() % cs->alph[15];
	while (start_state->third == start_state->second || start_state->third == start_state->first)
		start_state->third = cs->alph[0] + rand() % cs->alph[15];

	cs->decrypt->start_state = start_state;
	cs->encrypt->start_state = start_state;
	 
	return;
}


static void generate_rotor_value(ALPH rotor)
{
	BYTE tmp;
	BYTE count_free_base = 0x0f;
	BYTE num_el_array;

	BYTE base[] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

#if TESTMODE
	for (int i = 0; i < 16; ++i) {
#else
	for (int i = 1; i < 17; ++i) {
#endif
		num_el_array = 0 + rand() % (count_free_base + 1);

		rotor[i] = base[num_el_array];

		tmp = base[num_el_array];
		base[num_el_array] = base[count_free_base];
		base[count_free_base] = tmp;
		
		--count_free_base;
	}
}


static void init_rotors_table(PCRYPTOSYSTEM cs) 
{
	/*BYTE alf_for_base[] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};*/

#if TESTMODE
	
	//ALPH r1		= (ALPH)malloc(sizeof(BYTE) * 16);
	//ALPH r2		= (ALPH)malloc(sizeof(BYTE) * 16);
	//ALPH r3		= (ALPH)malloc(sizeof(BYTE) * 16);
	//ALPH refl	= (ALPH)malloc(sizeof(BYTE) * 16);
	
	cs->encrypt->reflector = cs->decrypt->reflector = (ALPH)malloc(sizeof(BYTE) * 16);
	//cs->decrypt->reflector = (ALPH)malloc(sizeof(BYTE) * 16);

	BYTE refl[16] = 
	{
		0x0a, 0x05, 0x07, 0x0f, 0x0d, 0x01, 0x0b, 0x02, 0x0e, 0x0c, 0x00, 0x06, 0x09, 0x04, 0x08, 0x03
	};

	//generate_rotor_value(r1);
	//generate_rotor_value(r2);
	//generate_rotor_value(r3);

	//cs->encrypt->rotor_1 = cs->decrypt->rotor_1 = r1;
	//cs->encrypt->rotor_2 = cs->decrypt->rotor_2 = r2;
	//cs->encrypt->rotor_3 = cs->decrypt->rotor_3 = r3;

	for(BYTE i = 0; i < 16; ++i)
		cs->encrypt->reflector[i] = cs->decrypt->reflector[i] = refl[i];

	//cs->decrypt->rotor_1 = r1;
	//cs->decrypt->rotor_2 = r2;
	//cs->decrypt->rotor_3 = r3;

#else
	
	//ALPH r1		= (ALPH)malloc(sizeof(BYTE) * 17);
	//ALPH r2		= (ALPH)malloc(sizeof(BYTE) * 17);
	//ALPH r3		= (ALPH)malloc(sizeof(BYTE) * 17);
	////ALPH refl	= (ALPH)malloc(sizeof(BYTE) * 16);

	//generate_rotor_value(alf_for_base, r1);
	//generate_rotor_value(alf_for_base, r2);
	//generate_rotor_value(alf_for_base, r3);

	//r1[0] = cs->encrypt->start_state->first;
	//r2[0] = cs->encrypt->start_state->second;
	//r3[0] = cs->encrypt->start_state->third;

	//cs->encrypt->rotor_1	= cs->decrypt->rotor_1 = r1;
	//cs->encrypt->rotor_2	= cs->decrypt->rotor_2 = r2;
	//cs->encrypt->rotor_3	= cs->decrypt->rotor_3 = r3;
	//cs->encrypt->reflector	= cs->decrypt->reflector = &refl;

#endif // TESTMODE

	return;
}

__declspec(dllexport)
PCRYPTOSYSTEM init_crypto()
{
	srand(time(NULL));

	BYTE alf_for_base[] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

	PCRYPTOSYSTEM cs = NULL;
	cs = realloc(cs, sizeof(CRYPTOSYSTEM));
	
	cs->encrypt	= (PMACHINE)malloc(sizeof(MACHINE));
	cs->decrypt	= (PMACHINE)malloc(sizeof(MACHINE));
	

	cs->alph = (ALPH)malloc(sizeof(BYTE) * 16);
	for (BYTE i = 0; i < 16; ++i)
		cs->alph[i] = i;

	generic_start_state(cs);

	/*cs->encrypt->curr_state = (PSTATE)malloc(sizeof(STATE));
	cs->decrypt->curr_state = (PSTATE)malloc(sizeof(STATE));*/
#if TESTMODE
	cs->encrypt->curr_state = (PSTATE)malloc(sizeof(STATE));
	cs->decrypt->curr_state = (PSTATE)malloc(sizeof(STATE));
	cs->encrypt->curr_state->first	 = cs->decrypt->curr_state->first	= cs->encrypt->start_state->first;
	cs->encrypt->curr_state->second	 = cs->decrypt->curr_state->second	= cs->encrypt->start_state->second;
	cs->encrypt->curr_state->third	 = cs->decrypt->curr_state->third	= cs->encrypt->start_state->third;

	//cs->decrypt->curr_state->first = cs->encrypt->start_state->first;
	//cs->decrypt->curr_state->second = cs->encrypt->start_state->second;
	//cs->decrypt->curr_state->third = cs->encrypt->start_state->third;

#endif // TESTMODE

	init_rotors_table(cs);

	return cs;
}

static BYTE MoveEncryptRotor_1(PCRYPTOSYSTEM cs) {
	return mod16(++GET_CURR_STATE(cs, encrypt, first));
}

static BYTE MoveEncryptRotor_2(PCRYPTOSYSTEM cs) {
	return mod16(++GET_CURR_STATE(cs, encrypt, second));
}

static BYTE MoveEncryptRotor_3(PCRYPTOSYSTEM cs) {
	return mod16(++GET_CURR_STATE(cs, encrypt, third));
}

__declspec(dllexport)
void MoveEncryptRotors(PCRYPTOSYSTEM cs) {
	if (MoveEncryptRotor_1(cs) == 0x00)
		if (MoveEncryptRotor_2(cs) == 0x00)
			if (MoveEncryptRotor_3(cs) == 0x00);
				
	return;
}


static BYTE IndexOf(ALPH rotor, BYTE find) {
	for (BYTE i = 0; i < 16; ++i)
		if (rotor[i] == find)
			return i;

	return 0;
}

__declspec(dllexport)
BYTE Encrypt(PCRYPTOSYSTEM cs, BYTE value) {
	MoveEncryptRotors(cs);

	BYTE res;

	//BYTE r1 = cs->encrypt->rotor_1[mod16(cs->encrypt->curr_state.first + value)];
	BYTE r1 = cs->encrypt->rotor_1[mod16(GET_CURR_STATE(cs, encrypt, first) + value)];
	BYTE r2 = cs->encrypt->rotor_2[mod16(r1 + mod16((GET_CURR_STATE(cs, encrypt, second) - GET_CURR_STATE(cs, encrypt, first))))];
	BYTE r3 = cs->encrypt->rotor_3[mod16(r2 + mod16((GET_CURR_STATE(cs, encrypt, third) - GET_CURR_STATE(cs, encrypt, second))))];

	BYTE refl = cs->encrypt->reflector[mod16(r3 - GET_CURR_STATE(cs, encrypt, third))];
	
	r3 = cs->alph[IndexOf(cs->encrypt->rotor_3, mod16(refl + GET_CURR_STATE(cs, encrypt, third)))];
	r2 = cs->alph[IndexOf(cs->encrypt->rotor_2, mod16(r3 - mod16((GET_CURR_STATE(cs, encrypt, third) - GET_CURR_STATE(cs, encrypt, second)))))];
	r1 = cs->alph[IndexOf(cs->encrypt->rotor_1, mod16(r2 - mod16((GET_CURR_STATE(cs, encrypt, second) - GET_CURR_STATE(cs, encrypt, first)))))];

	res = mod16(r1 - GET_CURR_STATE(cs, encrypt, first));

	return res;
}


static BYTE MoveDecryptRotor_1(PCRYPTOSYSTEM cs) {
	return mod16(++GET_CURR_STATE(cs, decrypt, first));
}

static BYTE MoveDecryptRotor_2(PCRYPTOSYSTEM cs) {
	return mod16(++GET_CURR_STATE(cs, decrypt, second));
}

static BYTE MoveDecryptRotor_3(PCRYPTOSYSTEM cs) {
	return mod16(++GET_CURR_STATE(cs, decrypt, third));
}

static void MoveDecryptRotors(PCRYPTOSYSTEM cs) {
	if (MoveDecryptRotor_1(cs) == 0x00)
		if (MoveDecryptRotor_2(cs) == 0x00)
			if (MoveDecryptRotor_3(cs) == 0x00);

	return;
}

__declspec(dllexport)
BYTE Decrypt(PCRYPTOSYSTEM cs, BYTE value) {
	MoveDecryptRotors(cs);

	BYTE r1 = cs->decrypt->rotor_1[mod16(GET_CURR_STATE(cs, decrypt, first) + value)];
	BYTE r2 = cs->decrypt->rotor_2[mod16(r1 + (GET_CURR_STATE(cs, decrypt, second) - GET_CURR_STATE(cs, decrypt, first)))];
	BYTE r3 = cs->decrypt->rotor_3[mod16(r2 + (GET_CURR_STATE(cs, decrypt, third) - GET_CURR_STATE(cs, decrypt, second)))];

	BYTE refl = cs->decrypt->reflector[mod16(r3 - GET_CURR_STATE(cs, decrypt, third))];

	r3 = cs->alph[IndexOf(cs->decrypt->rotor_3, mod16(refl + GET_CURR_STATE(cs, decrypt, third)))];
	r2 = cs->alph[IndexOf(cs->decrypt->rotor_2, mod16(r3 - (GET_CURR_STATE(cs, decrypt, third) - GET_CURR_STATE(cs, decrypt, second))))];
	r1 = cs->alph[IndexOf(cs->decrypt->rotor_1, mod16(r2 - mod16((GET_CURR_STATE(cs, decrypt, second) - GET_CURR_STATE(cs, decrypt, first)))))];

	BYTE res = mod16(r1 - GET_CURR_STATE(cs, decrypt, first));

	return res;
}

//---------------------------------------------------------