import sys
import random

import ctypes
import struct

enigmaModule = ctypes.WinDLL('../Enigma/bin/x64/enigma64.dll')
#enigmaModule = ctypes.WinDLL('../Enigma/bin/enigma32.dll', True)

#------------------------------------------------
#       STRUCTURES

class STATE(ctypes.Structure):
    _fields_ = [('first',   ctypes.c_ubyte),
               ('second',   ctypes.c_ubyte),
               ('third',    ctypes.c_ubyte)]


class MACHINE(ctypes.Structure):
    _fields_ = [('start_state', ctypes.POINTER(STATE)),
               ('alph',         ctypes.POINTER(ctypes.c_ubyte)),
               ('rotor_1',      ctypes.POINTER(ctypes.c_ubyte)),
               ('rotor_2',      ctypes.POINTER(ctypes.c_ubyte)),
               ('rotor_3',      ctypes.POINTER(ctypes.c_ubyte)),
               ('curr_state',   ctypes.POINTER(STATE))]
                

class CRYPTOSYSTEM(ctypes.Structure):
    _fields_ = [('alph',    ctypes.POINTER(ctypes.c_ubyte)),
                ('encrypt', ctypes.POINTER(MACHINE)),
                ('decrypt', ctypes.POINTER(MACHINE))]
   
    

#------------------------------------------------
#       FUNCTIONS

# PCRYPTOSYSTEM init_crypto()
enigmaModule.init_crypto.restype = ctypes.POINTER(CRYPTOSYSTEM)
enigmaModule.init_crypto.argtypes = [ctypes.c_void_p]

# BYTE Encrypt(PCRYPTOSYSTEM cs, BYTE value);
enigmaModule.Encrypt.restype = ctypes.c_ubyte
enigmaModule.Encrypt.argtypes = [ctypes.POINTER(CRYPTOSYSTEM), ctypes.c_ubyte]

# BYTE Decrypt(PCRYPTOSYSTEM cs, BYTE value);
enigmaModule.Decrypt.restype = ctypes.c_ubyte
enigmaModule.Decrypt.argtypes = [ctypes.POINTER(CRYPTOSYSTEM), ctypes.c_ubyte]

# void CsSetRotors(PCRYPTOSYSTEM cs, ALPH rotor1, ALPH rotor2, ALPH rotor3);
enigmaModule.CsSetRotors.argtypes = [ctypes.POINTER(CRYPTOSYSTEM), ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_ubyte)]


# void MoveEncryptRotors(PCRYPTOSYSTEM cs)
enigmaModule.MoveEncryptRotors.argtypes = [ctypes.POINTER(CRYPTOSYSTEM)]
