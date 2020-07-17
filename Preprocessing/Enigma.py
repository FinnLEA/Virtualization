import sys
import random

from ctypes import *
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
               ('reflector',   ctypes.POINTER(ctypes.c_ubyte)),
               ('curr_state',   ctypes.POINTER(STATE))]
                

class CRYPTOSYSTEM(ctypes.Structure):
    _fields_ = [('alph',    ctypes.POINTER(ctypes.c_ubyte)),
                ('encrypt', ctypes.POINTER(MACHINE)),
                ('decrypt', ctypes.POINTER(MACHINE))]
   
    

#------------------------------------------------
#       FUNCTIONS

# PCRYPTOSYSTEM init_crypto()
enigmaModule.init_crypto.restype = POINTER(CRYPTOSYSTEM)
enigmaModule.init_crypto.argtypes = [ctypes.c_void_p]

# BYTE Encrypt(PCRYPTOSYSTEM cs, BYTE value);
enigmaModule.Encrypt.restype = c_ubyte
enigmaModule.Encrypt.argtypes = [POINTER(CRYPTOSYSTEM), c_ubyte]

# BYTE Decrypt(PCRYPTOSYSTEM cs, BYTE value);
enigmaModule.Decrypt.restype = c_ubyte
enigmaModule.Decrypt.argtypes = [POINTER(CRYPTOSYSTEM), c_ubyte]

# void CsSetRotors(PCRYPTOSYSTEM cs, ALPH rotor1, ALPH rotor2, ALPH rotor3);
enigmaModule.CsSetRotors.argtypes = [POINTER(CRYPTOSYSTEM), POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte)]


# void MoveEncryptRotors(PCRYPTOSYSTEM cs)
enigmaModule.MoveEncryptRotors.argtypes = [ctypes.POINTER(CRYPTOSYSTEM)]



def mod16(value:c_ubyte) -> c_ubyte:
    return ((value % 16) + 16) % 16

def MoveRotor_1(state:POINTER(STATE)) -> c_ubyte:
    state.contents.first += 1
    state.contents.first = mod16(state.contents.first)
    return state.contents.first

def MoveRotor_2(state:POINTER(STATE)) -> c_ubyte:
    state.contents.second += 1
    state.contents.second = mod16(state.contents.second)
    return state.contents.second

def MoveRotor_3(state:POINTER(STATE)) -> c_ubyte:
    state.contents.third += 1
    state.contents.third = mod16(state.contents.third)
    return state.contents.third

def MoveEncryptRotors(cs:POINTER(CRYPTOSYSTEM)):
    
    curr_st = cs.contents.encrypt.contents.curr_state
    if MoveRotor_1(curr_st) == 0:
        if MoveRotor_2(curr_st) == 0:
            if MoveRotor_3(curr_st) == 0:
                return
    
    return


