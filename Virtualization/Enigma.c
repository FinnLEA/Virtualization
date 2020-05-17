#include "Enigma.h"

//void generic_keys() {
//	srand(time(NULL));
//
//	BYTE alf_for_keys[] =
//	{
//		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
//	};
//
//	cs.keys.first = alf_for_keys[0] + rand() % alf_for_keys[15];
//	cs.keys.second = alf_for_keys[0] + rand() % alf_for_keys[15];
//	if (cs.keys.first == cs.keys.second)
//		cs.keys.second = alf_for_keys[0] + rand() % alf_for_keys[15];
//
//	cs.keys.third = alf_for_keys[0] + rand() % alf_for_keys[15];
//	if (cs.keys.third == cs.keys.second || cs.keys.third == cs.keys.first)
//		cs.keys.third = alf_for_keys[0] + rand() % alf_for_keys[15];
//
//}


//
//
//static void generic_keys()
//{
//	srand(time(NULL));
//
//	BYTE alf_for_keys[] =
//	{
//		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
//	};
//
//	cs.keys.first = alf_for_keys[0] + rand() % alf_for_keys[15];
//	cs.keys.second = alf_for_keys[0] + rand() % alf_for_keys[15];
//	if(cs.keys.first == cs.keys.second)
//		cs.keys.second = alf_for_keys[0] + rand() % alf_for_keys[15];
//
//	cs.keys.third = alf_for_keys[0] + rand() % alf_for_keys[15];
//	if (cs.keys.third == cs.keys.second || cs.keys.third == cs.keys.first)
//		cs.keys.third = alf_for_keys[0] + rand() % alf_for_keys[15];
//}

static void generic_start_state(PCRYPTOSYSTEM cs)
{
	
	PSTATE start_state = (PSTATE)malloc(sizeof(STATE));

	start_state->first = alf_for_keys[0] + rand() % alf_for_keys[15];
	start_state->second = alf_for_keys[0] + rand() % alf_for_keys[15];
	while (start_state->first == start_state->second)
		start_state->second = alf_for_keys[0] + rand() % alf_for_keys[15];

	start_state->third = alf_for_keys[0] + rand() % alf_for_keys[15];
	while (start_state->third == start_state->second || start_state->third == start_state->first)
		start_state->third = alf_for_keys[0] + rand() % alf_for_keys[15];

	cs->decrypt->start_state = start_state;
	cs->enctypt->start_state = start_state;

	return;
}

static void generate_rotor_value( __in ALPH base, __inout ALPH rotor)
{
	BYTE tmp;
	BYTE count_free_base = 0x0f;
	BYTE num_el_array;

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
	BYTE alf_for_base[] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

#if TESTMODE
	
	ALPH r1		= (ALPH)malloc(sizeof(BYTE) * 16);
	ALPH r2		= (ALPH)malloc(sizeof(BYTE) * 16);
	ALPH r3		= (ALPH)malloc(sizeof(BYTE) * 16);
	//ALPH refl	= (ALPH)malloc(sizeof(BYTE) * 16);
	
	BYTE refl[16] = 
	{
		0x0a, 0x05, 0x07, 0x0f, 0x0d, 0x01, 0x0b, 0x02, 0x0e, 0x0c, 0x00, 0x06, 0x09, 0x04, 0x08, 0x03
	};

	generate_rotor_value(alf_for_base, r1);
	generate_rotor_value(alf_for_base, r2);
	generate_rotor_value(alf_for_base, r3);

	cs->enctypt->rotor_1	= cs->decrypt->rotor_1 = r1;
	cs->enctypt->rotor_2	= cs->decrypt->rotor_2 = r2;
	cs->enctypt->rotor_3	= cs->decrypt->rotor_3 = r3;
	cs->enctypt->reflector	= cs->decrypt->reflector = &refl;

	//cs->decrypt->rotor_1 = r1;
	//cs->decrypt->rotor_2 = r2;
	//cs->decrypt->rotor_3 = r3;

#else
	
	ALPH r1		= (ALPH)malloc(sizeof(BYTE) * 17);
	ALPH r2		= (ALPH)malloc(sizeof(BYTE) * 17);
	ALPH r3		= (ALPH)malloc(sizeof(BYTE) * 17);
	//ALPH refl	= (ALPH)malloc(sizeof(BYTE) * 16);

	generate_rotor_value(alf_for_base, r1);
	generate_rotor_value(alf_for_base, r2);
	generate_rotor_value(alf_for_base, r3);

	r1[0] = cs->enctypt->start_state->first;
	r2[0] = cs->enctypt->start_state->second;
	r3[0] = cs->enctypt->start_state->third;

	cs->enctypt->rotor_1	= cs->decrypt->rotor_1 = r1;
	cs->enctypt->rotor_2	= cs->decrypt->rotor_2 = r2;
	cs->enctypt->rotor_3	= cs->decrypt->rotor_3 = r3;
	cs->enctypt->reflector	= cs->decrypt->reflector = &refl;

#endif // TESTMODE

	return;
}


PCRYPTOSYSTEM init_crypto()
{
	srand(time(NULL));

	BYTE alf_for_base[] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

	PCRYPTOSYSTEM cs = (PCRYPTOSYSTEM)malloc(sizeof(CRYPTOSYSTEM));
	cs->enctypt	= (PENCRYPT)malloc(sizeof(ENCRYPT));
	cs->decrypt	= (PDECRYPT)malloc(sizeof(DECRYPT));

	cs->enctypt->alph = cs->decrypt->alph = &alf_for_base;

	generic_start_state(cs);

	cs->enctypt->curr_state = (PSTATE)malloc(sizeof(STATE));
	cs->decrypt->curr_state = (PSTATE)malloc(sizeof(STATE));
#if TESTMODE

	cs->enctypt->curr_state->first	 = cs->decrypt->curr_state->first	= cs->enctypt->start_state->first;
	cs->enctypt->curr_state->second	 = cs->decrypt->curr_state->second	= cs->enctypt->start_state->second;
	cs->enctypt->curr_state->third	 = cs->decrypt->curr_state->third	= cs->enctypt->start_state->third;

	//cs->decrypt->curr_state->first = cs->enctypt->start_state->first;
	//cs->decrypt->curr_state->second = cs->enctypt->start_state->second;
	//cs->decrypt->curr_state->third = cs->enctypt->start_state->third;

#endif // TESTMODE

	init_rotors_table(cs);

	return cs;
}
