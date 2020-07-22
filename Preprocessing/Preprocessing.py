#------------------------------------------------------------------------------
#
#   Скрипт препроцессора виртуальной машины
#
#------------------------------------------------------------------------------


import re
import sys
from Enigma import *

from tables import *

from ctypes import *
import struct




#----------------------------------------------------------
#       Global variables
#----------------------------

base = [
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	   ]


optypes = { 
            'reg'         : 0x01,
            'constaddr'   : 0x02,
            'const'       : 0x03,
            'regaddr'     : 0x04,
            'useraddr'    : 0x05
          }

regnum = { 
           '0'  : 0x01,
           '1'  : 0x02,
           '2'  : 0x03,
           '3'  : 0x04,
           '4'  : 0x05,
           '5'  : 0x06,
           '6'  : 0x07
         }

opcodes = { 
            'MOV'   : 0x01, 
            'AND'   : 0x02,
            'OR'    : 0x03, 
            'CALL'  : 0x04,
            'ADD'   : 0x05,
            'SUB'   : 0x06,
            'XOR'   : 0x07,
            'MUL'   : 0x08,
            'DIV'   : 0x09,
            'PUSH'  : 0x0A,
            'POP'   : 0x0B
          }

preffs = {
            'ENIGMA'    : 0xe3,
            'STATIC'    : 0x40
         }

busyOpcodes = [0xbe, 0xac, 0x05, 0x4c, 0xb0]

countOperands = 2
szDefineOpsFunc = ''
oldPref = 0
instructionPref = 0xe3 # standart pref for Enigma code
flagStart = False

cs = None

mode = 0  # 0 - Enigma, 1 - Static

indexForStatic = 0

#----------------------------------------------------------


#----------------------------------------------------------
#       Functions
#----------------------------

def EncryptByte(byte:int) -> int:
    global cs
    encryptByte = enigmaModule.Encrypt(cs, c_ubyte(byte))

    #MoveEncryptRotors(cs)
    return encryptByte


#----------------------------------------------------------
#      Parse and insert instructions

def EncryptOpcode(opcode:int) -> int:
    if mode == 0:
        opcode = EncryptByte(opcode)
        opcode <<= 4
        opcode |= random.randint(0, 0x0f)
        for val in busyOpcodes:
            while val == opcode:
                opcode |= random.randint(0, 0x0f)
    else:
        opcode = staticOpcodes[indexForStatic][opcode - 1]
    
    return opcode

def InsertOpcode(instrName:str) -> str:
    global countOperands
    if instrName == 'PUSH' or instrName == 'POP' or instrName == 'CALL':
        countOperands = 1
    else:
        countOperands = 2
        
    opcode = opcodes[instrName]
    opcode = EncryptOpcode(opcode)
    outStr = '__asm _emit 0x'
    byte = opcode.to_bytes(1, byteorder='big')
    outStr += byte.hex() + '\n'
    return outStr


#--------------------------------------
#      Parse and insert preffix

def InsertPref():
    exPref = instructionPref
    if mode == 1:        
        global indexForStatic
        indexForStatic = random.randint(0, 0x0f)
        exPref |= indexForStatic

    byte = exPref.to_bytes(1, byteorder='big')
    outStr = '__asm _emit 0x'
    outStr += byte.hex() + '\n'
    return outStr
    

#----------------------------------------
#      Parse and insert start instruction

def CopyState(machine:POINTER(MACHINE)):
    machine.contents.curr_state.contents.first = machine.contents.start_state.contents.first
    machine.contents.curr_state.contents.second = machine.contents.start_state.contents.second
    machine.contents.curr_state.contents.third = machine.contents.start_state.contents.third

    return


def ParseInitVM(Buf:str) -> str:
    global flagStart
    flagStart = True
    global cs
    matches = re.findall(r'([_]{1}[0-9]{1,}[_][a-zA-Z]*)', Buf)
    arg1 = matches[0]
    arg2 = matches[1]

    outBuf = '_BEGIN_PROTECT_' + '(' + arg1 + ',' + arg2 + ')\n{'
    outBuf += '__try {\n'
    outBuf += '__asm ud2\n'
    outBuf += '__asm _emit 0x43\n'
    
    cs = enigmaModule.init_crypto(None)
    CopyState(cs.contents.encrypt)
    
    ind = random.randint(0, 4)
    byte = busyOpcodes[ind].to_bytes(1, byteorder='big')

    outBuf += '__asm _emit 0x' + byte.hex() + '\n'
    dec = cs.contents.encrypt
    #st = dec.contents.start_state.contents.first

    byte1 = int(dec.contents.start_state.contents.first)
    byte1 <<= 4
    halfByte1 = random.randint(0, 0x0f)
    byte1 |= halfByte1
    byte = byte1.to_bytes(1, byteorder='big')
    outBuf += '__asm _emit 0x' + byte.hex() + '\n'

    byte2 = int(dec.contents.start_state.contents.second)
    byte2 <<= 4
    halfByte2 = random.randint(0, 0x0f)
    while halfByte2 == (byte1 >> 4):
        halfByte2 = random.randint(0, 0x0f)
    byte2 |= halfByte2
    byte = byte2.to_bytes(1, byteorder='big')
    outBuf += '__asm _emit 0x' + byte.hex() + '\n'

    byte3 = int(dec.contents.start_state.contents.third)
    byte3 <<= 4
    halfByte3 = random.randint(0, 0x0f)
    while halfByte3 == (byte2 >> 4) and halfByte3 == (byte1 >> 4):
        halfByte3 = random.randint(0, 0x0f)
    byte3 |= halfByte3
    byte = byte3.to_bytes(1, byteorder='big')
    outBuf += '__asm _emit 0x' + byte.hex() + '\n'

    enigmaModule.CsSetRotors(cs, ctypes.cast(valuesForRotors[halfByte1], ctypes.POINTER(ctypes.c_ubyte)),
                             ctypes.cast(valuesForRotors[halfByte2], ctypes.POINTER(ctypes.c_ubyte)),
                             ctypes.cast(valuesForRotors[halfByte3], ctypes.POINTER(ctypes.c_ubyte)))

    return outBuf


#--------------------------------------
#      Parse and insert end of VM block

def ParseEndVM(buf:str) -> str:
    outBuf = '}\n__except(Handler(GetExceptionInformation())){\n'
    outBuf += '_vm_destruct_(vm);\n}\n}'


    return outBuf


#--------------------------------------
#      Parse and insert operants

opsDef = []

def InsertOperandDefinition() -> str:
    global szDefineOpsFunc
    szDefineOpsFunc = ''
    if len(opsDef) == 2:
        szDefineOpsFunc += opsDef[1]
        szDefineOpsFunc += opsDef[0]
    elif len(opsDef) == 1:
        szDefineOpsFunc += opsDef[0]
    else:
        szDefineOpsFunc = ''

    return szDefineOpsFunc

def RememberOperandDefinition(szOp:str, type:int, encType:int = 0, isHex = False) -> str:   
    outBuf = ''
    if type == optypes['reg'] or type == optypes['regaddr']:
        #outBuf += 'define_operand(vm,' + str(type) + ',' + str(int(szOp) + 1) + ');\n'
        return outBuf
    else:
        if isHex == True:
            value = int(szOp, 16)
        else:
            value = int(szOp)
        resValue = 0
        for i in range(4):
            #resValue <<= 8
            rotate = (0xff << (i * 8))
            if (value & rotate) != 0:
                resValue |= ((((value & rotate) >> (i * 8)) ^ type) << (i * 8))
            #value <<= 8

        outBuf += '_push_(vm,'
        if isHex == True:
            resBytes = resValue.to_bytes(4, 'big')
            resBuf = resBytes.hex()
            outBuf += '0x' + resBuf + ');\n'
        else:
            resBuf = str(resValue)
            outBuf += resBuf + ');\n'
    
    opsDef.append(outBuf)

    return 

def EncryptOperandType(byte:int) -> int:
    if mode == 0:
        encryptedByte = EncryptByte(byte)
    elif mode == 1:
        encryptedByte = staticOpTypes[indexForStatic][byte - 1]

    if byte == optypes['const'] or byte == optypes['constaddr']:
            encryptedByte <<= 4
            encryptedByte |= random.randint(0, 0x0f)
    return encryptedByte

def InsertOptype(buf:str) -> str:
    out = ''
    global szDefineOpsFunc
    op = re.findall(r'\[r([0-9]+)\]', buf)
    if len(op) != 0:
        type = optypes['regaddr']
        tmp = op[0]
        tmp = int(tmp)
        tmp += 1
        #exType = type
        #szDefineOpsFunc += InsertOperandDefinition(op[0], type)
        RememberOperandDefinition(op[0], type)
        type = EncryptOperandType(type)
        
        type <<= 4
        type |= tmp
        byte = type.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte.hex() + '\n'
        return out

    op = re.findall(r'[rR]([0-9])', buf)
    if len(op) != 0:
        type = optypes['reg']
        tmp = op[0]
        tmp = int(tmp)
        tmp += 1
       # szDefineOpsFunc += InsertOperandDefinition(op[0], type)
        RememberOperandDefinition(op[0], type)
        type = EncryptOperandType(type)
        type <<= 4
        type |= tmp
        byte = type.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte.hex() + '\n'
        return out

    op = re.findall(r'\[0x([a-fA-F0-9]+)\]', buf)
    if len(op) != 0:
        type = optypes['constaddr']
        value = op[0]
        exType = type
        type = EncryptOperandType(exType)
        RememberOperandDefinition(value, exType, type, True)
        #szDefineOpsFunc += InsertOperandDefinition(value, type, True)
        szFirstByte = ''

        
        byte1 = type.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte1.hex() + '\n'
        
        if len(value) > 1:
            szFirstByte = value[len(value) - 2] + value[len(value) - 1]
        else:
            szFirstByte = '0' + value[len(value) - 1]
        
        bFirstByte = int(szFirstByte, 16)
        byte2 = bFirstByte.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte2.hex() + '\n'
        return out

    op = re.findall(r'\[([a-fA-F0-9]+)\]', buf)
    if len(op) != 0:
        type = optypes['constaddr']
        exType = type
        type = EncryptOperandType(exType)
        byte1 = type.to_bytes(1, 'big')
        
        out += '__asm _emit 0x'
        out += byte1.hex() + '\n'
        value = op[0]
        RememberOperandDefinition(value, exType, type, False)
        #szDefineOpsFunc += InsertOperandDefinition(value, type, False)
        szFirstByte = ''
        if len(value) > 1:
            szFirstByte = value[len(value) - 2] + value[len(value) - 1]
        else:
            szFirstByte = '0' + value[len(value) - 1]
        
        bFirstByte = int(szFirstByte, 10)
        byte2 = bFirstByte.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte2.hex() + '\n'
        return out

    op = re.findall(r'0x([0-9a-fA-F]+)', buf)
    if len(op) != 0:
        type = optypes['const']
        exType = type
        type = EncryptOperandType(exType)
        RememberOperandDefinition(op[0], exType, type, True)
        #szDefineOpsFunc += InsertOperandDefinition(op[0], type, True)
        byte = type.to_bytes(1, 'big')
        
        out += '__asm _emit 0x'
        out += byte.hex() + '\n'
        return out

    op = re.findall(r'([0-9a-fA-F]+)', buf)
    if len(op) != 0:
        type = optypes['const']
        exType = type
        type = EncryptOperandType(exType)
        RememberOperandDefinition(op[0], exType, type, False)
        #szDefineOpsFunc += InsertOperandDefinition(op[0], type, False)
        byte = type.to_bytes(1, 'big')
        
        out += '__asm _emit 0x'
        out += byte.hex() + '\n'
        return out

    return ''

def ParseOperands(buf:str) -> str:
    outBuf = ''
    ops = []
    
    if countOperands == 1:
       outBuf += InsertOptype(buf)
    elif countOperands == 2:
       ops = re.findall(r'[ ]{1,}([0-9a-z \[\]x]+), *([0-9a-z \[\]x]+)', buf)
       if len(ops) == 0:
           return ''
       ops = ops[0]
       op1 = ops[0]
       op2 = ops[1]
       outBuf += InsertOptype(op1)
       outBuf += InsertOptype(op2)

   # '[ ]{1,}([0-9a-z \[\]x]+), *([0-9a-z \[\]x]+)'

    InsertOperandDefinition()
    opsDef.clear()

    return outBuf


#--------------------------------------
#      Insert full instructions

def InsertInstruction(buf:str) -> str:
    global szDefineOpsFunc
    szDefineOpsFunc = ''
    outBuf = ''
    instruction = re.findall(r'/*(vm|VM)[_]{1}([A-Za-z]{2,})', buf)
   # i = len(instruction)
    #spl = buf.rsplit('VM_', 1)
  #  while i != 0:
    tmp = instruction[0]
    if tmp[0] == 'VM':
        instrName = tmp[1]
        outBuf += '__asm ud2\n'
        outBuf += InsertPref()
        outBuf += InsertOpcode(instrName)
        outBuf += ParseOperands(buf)
        #i -= 1
    
    outBuf = szDefineOpsFunc + outBuf
    
    return outBuf


#--------------------------------------
#       Handle Static code

def SetMode(newMode:int):
    global instructionPref
    global oldPref
    global mode

    mode = newMode
    if newMode == 1:
        oldPref = instructionPref
        instructionPref = preffs['STATIC']
    elif newMode == 0:
        if oldPref == preffs['ENIGMA']:
            instructionPref = oldPref
        else:
            instructionPref = preffs['ENIGMA']

    return

def ParseStaticCode(buf:str) -> str:
    outBuf = ''
    
    SetMode(1)

    outBuf += buf

    return outBuf


#--------------------------------------
#       Handle Cycle

def ParseCycle(buf:str) -> str:
    outBuf = ''
    #global instructionPref
    #instructionPref -= 2

    SetMode(1)

    outBuf += buf

    return outBuf

def ParseEndCycle(buf:str) -> str:
    outBuf = ''
    #global instructionPref
    #instructionPref += 2

    SetMode(0)

    outBuf += buf
    
    return outBuf


#--------------------------------------
#       Handle If - Else

def ParseEndStaticCode(buf:str) -> str:
    outBuf = ''
    
    SetMode(0)    

    outBuf += buf
    
    return outBuf


def ParseIfElse(buf:str, Mode:int) -> str:
    outBuf = ''

    SetMode(not Mode)

    outBuf += buf

    return outBuf


#--------------------------------------
#       Pasre file

def ParseFile(nameFile):
    
    #fdBuf = fdFrom.read()
    #matches = re.findall(r'(BEGIN_PROTECT | )', fdBuf)
    try:
        fdFrom = open(nameFile, "r")
        fdIn = open(nameFile[:-2] + "_1.c", "w")
        fdBuf = ""
        tmp = ""
        for string in fdFrom:       
            tmp = string

            if tmp.find('//', 0) != -1:
                continue
            if tmp.find('BEGIN_PROTECT', 0) != -1:
                fdBuf += ParseInitVM(tmp)
            elif tmp.find('VM_', 0) != -1 and flagStart == True:
                fdBuf += InsertInstruction(tmp)
            elif (tmp.find('_While', 0) != -1 or tmp.find('_For', 0) !=-1) and flagStart == True:
                fdBuf += ParseCycle(tmp)
            elif (tmp.find('STATIC_CODE', 0) != -1 or tmp.find('static_code', 0) != -1) and flagStart == True:
                fdBuf += ParseStaticCode(tmp)
            elif (tmp.find('ENDS', 0) != -1 or tmp.find('ends', 0) != -1) and flagStart == True:
                 fdBuf += ParseEndStaticCode(tmp)
            elif (tmp.find('_Endw', 0)!= -1 or tmp.find('_endfor', 0) != -1) and flagStart == True:
                fdBuf += ParseEndCycle(tmp)
            elif tmp.find('_If', 0) != -1  and flagStart == True:
                 fdBuf += ParseIfElse(tmp, 0)
            elif tmp.find('_Elif', 0) != -1 and flagStart == True:
                fdBuf += ParseIfElse(tmp , 0)
            elif (tmp.find('_endif', 0) != -1 or tmp.find('_ENDIF', 0) != -1) and flagStart == True:
                fdBuf += ParseIfElse(tmp, 1)
            elif tmp.find('END_PROTECT', 0) != -1 and flagStart == True:
                fdBuf += ParseEndVM(tmp)
            else:
                fdBuf += tmp
            print(fdBuf)
    except:
        fdIn.close()
        fdFrom.close() 
        return
    finally:
    #print(fdBuf)
        fdIn.write(fdBuf)
        fdIn.close()
        fdFrom.close()
        return



def GenerateRotorValue():
    valueArray = list()
    
    countFreeBase = 0xff
    global base
    alf = range(0, 0x100)
    local_base = list(alf)
    print('[', end =' ')
    for val in range(11):                     
        num_el_array = random.randint(0, countFreeBase)
        value = local_base[num_el_array]
        
        for i in busyOpcodes:
            while value == i:
                num_el_array = random.randint(0, countFreeBase)
                value = local_base[num_el_array]

        valueArray.append(value)
        tmp = local_base[num_el_array]
        local_base[num_el_array] = local_base[countFreeBase]
        local_base[countFreeBase] = tmp

        countFreeBase -= 1
        
        byte = valueArray[val]
        byte = byte.to_bytes(1, 'big')
        print('0x',byte.hex(),', ', sep = '', end='')
    
    print('],')
    return


def main():

    for param in sys.argv:
        ParseFile(param)
        
    #cs = enigmaModule.init_crypto(None)
    #CopyState(cs.contents.encrypt)
    #enigmaModule.CsSetRotors(cs, ctypes.cast(valuesForRotors[9], ctypes.POINTER(ctypes.c_ubyte)),
    #                         ctypes.cast(valuesForRotors[7], ctypes.POINTER(ctypes.c_ubyte)),
    #                         ctypes.cast(valuesForRotors[0x0b], ctypes.POINTER(ctypes.c_ubyte)))
    #for i in range(16):
    #    resenc = enigmaModule.Encrypt(cs, c_ubyte(i))
    #    resdec = enigmaModule.Decrypt(cs, c_ubyte(resenc))
    #    print(i, ' -> ', resenc, ' -> ', resdec)
    #for i in range(0, 0x0f):
    #    GenerateRotorValue() 
    

    return 0
    

 

if __name__ == "__main__":
    main()
