import re
import sys
from Enigma import *

from tables import *

from ctypes import *
import struct



base = [
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	   ]

#--------------------------------------

instructionPattern = { 
                       'VM_MOV', 
                       'VM_AND',
                       'VM_OR',
                       'VM_XOR',
                       'VM_ADD',
                       'VM_SUB' 
                     }

optypes = { 
            'reg'         : 0x01,
            'constaddr'   : 0x02,
            'const'       : 0x03,
            'regaddr'     : 0x04
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
            'STATIC'    : 0x4a
         }

busyOpcodes = [0xbe, 0xac, 0x05, 0x4c, 0xb0]

countOperands = 2
szDefineOpsFunc = ''
instructionPref = 0xe3 # standart pref for Enigma code
flagStart = False

cs = None



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

def EncryptByte(byte:int) -> int:
    global cs
    encryptByte = enigmaModule.Encrypt(cs, c_ubyte(byte))
    #encryptByte1 = enigmaModule.Decrypt(cs, c_ubyte(encryptByte))
   #enigmaModule.MoveEncryptRotors(cs)
    #MoveEncryptRotors(cs)
    return encryptByte


#--------------------------------------
#      Parse and insert instructions

def EncryptOpcode(opcode:int) -> int:

    opcode = EncryptByte(opcode)
    opcode <<= 4
    opcode |= random.randint(0, 0x0f)
    for val in busyOpcodes:
        while val == opcode:
            opcode |= random.randint(0, 0x0f)

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
    byte = instructionPref.to_bytes(1, byteorder='big')
    outStr = '__asm _emit 0x'
    outStr += byte.hex() + '\n'
    return outStr
    

#
#   Проверяет на наличие нужной строки
#
def FindInstruction(str, fdBuf):
    for instr in instructionPattern:
        if str.find(instr) != -1:
            return True
    return False          


#----------------------------------------
#      Parse and insert start instruction

def CopyState(machine:POINTER(MACHINE)):
    machine.contents.curr_state.contents.first = machine.contents.start_state.contents.first
    machine.contents.curr_state.contents.second = machine.contents.start_state.contents.second
    machine.contents.curr_state.contents.third = machine.contents.start_state.contents.third

    return


def ParseInitVM(Buf:str):
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

def ParseEndVM(buf:str):
    outBuf = '}\n__except(Handler(GetExceptionInformation())){\n'
    outBuf += '_vm_destruct_(vm);\n}\n}'


    return outBuf

#--------------------------------------
#      Parse and insert operants

opsDef = []

def InsertOperandDefinition() -> str:
    #outBuf = ''
    #if type == optypes['reg'] or type == optypes['regaddr']:
    #    #outBuf += 'define_operand(vm,' + str(type) + ',' + str(int(szOp) + 1) + ');\n'
    #    return outBuf
    #else:
    #    #outBuf += 'define_operand(vm,' + str(type) + ','
    #    #if isHex:
    #    #    outBuf += '0x' + szOp + ');\n'
    #    #else:
    #    #    outBuf += szOp + ');\n'
    #    outBuf += '_push_(vm,'
    #    if isHex == True:
    #        outBuf += '0x' + szOp + ');\n'
    #    else:
    #        outBuf += szOp + ');\n'
    
    #opsDef.append(outBuf)
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
        #outBuf += 'define_operand(vm,' + str(type) + ','
        #if isHex:
        #    outBuf += '0x' + szOp + ');\n'
        #else:
        #    outBuf += szOp + ');\n'
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
    encryptedByte = EncryptByte(byte)
    if byte == optypes['const'] or byte == optypes['constaddr']:
        encryptedByte <<= 4
        encryptedByte |= random.randint(0, 0x0f)

    return encryptedByte

def InsertOptype(buf:str):
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

def ParseOperands(buf:str):
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

def InsertInstruction(buf:str):
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

def ParseCycle(buf:str) -> str:
    outBuf = ''
    global instructionPref
    instructionPref -= 2

    outBuf += buf

    return outBuf

#
#   Читает файл в буфер
#
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
          #pos = tmp.find('BEGIN_PROTECT', 0)
            if tmp.find('//', 0) != -1:
                continue
            if tmp.find('BEGIN_PROTECT', 0) != -1:
                fdBuf += ParseInitVM(tmp)
          #pos = tmp.find('VM_', 0)
            elif tmp.find('VM_', 0) != -1:
                fdBuf += InsertInstruction(tmp)
            elif tmp.find('_While', 0) != -1 and flagStart == True:
                fdBuf += ParseCycle(tmp)
            elif tmp.find('END_PROTECT', 0) != -1:
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



#def GenerateRotorValue():
#    valueArray = list()
    
#    countFreeBase = 0x0f
#    global base
#    print('[', end =' ')
#    for val in range(16):
        
#        num_el_array = random.randint(0, countFreeBase)
#        valueArray.append(base[num_el_array])

#        tmp = base[num_el_array]
#        base[num_el_array] = base[countFreeBase]
#        base[countFreeBase] = tmp

#        countFreeBase -= 1
        
#        byte = valueArray[val]
#        byte = byte.to_bytes(1, 'big')
#        print('0x',byte.hex(),', ', sep = '', end='')
    
#    print('],')
#    return


def main():
    #for file in sys.argv:\

    #cs = enigmaModule.init_crypto(None)
    #CopyState(cs.contents.encrypt)
    #enigmaModule.CsSetRotors(cs, ctypes.cast(valuesForRotors[9], ctypes.POINTER(ctypes.c_ubyte)),
    #                         ctypes.cast(valuesForRotors[7], ctypes.POINTER(ctypes.c_ubyte)),
    #                         ctypes.cast(valuesForRotors[0x0b], ctypes.POINTER(ctypes.c_ubyte)))

    #for i in range(16):
    #    resenc = enigmaModule.Encrypt(cs, c_ubyte(i))
    #    resdec = enigmaModule.Decrypt(cs, c_ubyte(resenc))
    #    print(i, ' -> ', resenc, ' -> ', resdec)
    
    ParseFile("../Virtualization/main.c")
    return 0
    

 

if __name__ == "__main__":
    main()
