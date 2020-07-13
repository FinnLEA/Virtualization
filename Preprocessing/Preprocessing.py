import re
import sys
from Enigma import *

from ctypes import *
import struct

#enigmaModule = ctypes.CDLL('./Enigma/enigma32.dll')

#class STATE(ctypes.Structure):
#    _filed_ = [('first',    ctypes.c_char),
#               ('second',   ctypes.c_char),
#               ('third',    ctypes.c_char)]

#class MACHINE(ctypes.Structure):
#    _field_ = [('start_state',  ctypes.POINTER(STATE)),
#               ('alph',         ctypes.POINTER(ctypes.c_char)),
#               ('rotor_1',      ctypes.POINTER(ctypes.c_char)),
#               ('rotor_2',      ctypes.POINTER(ctypes.c_char)),
#               ('rotor_3',      ctypes.POINTER(ctypes.c_char))
#               ('curr_state',   ctypes.Structure(STATE))]
                


#class CRYPTOSYSTEM(ctypes.Structure):
#    _fields_ = [('alph',    ctypes.POINTER(ctypes.c_char)),
#                ('encrypt', ctypes.POINTER(MACHINE)),
#                ('decrypt', ctypes.POINTER(MACHINE))]

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

countOperands = 2
szDefineOpsFunc = ''
instructionPref = 0x00

cs = None

#--------------------------------------

#
#   Парсинг строки
#
# def ParseLine(str):
#     r = 0xffab
#     bytes = r.to_bytes(2, byteorder='big')
#     res = bytes.hex()

#     print(bytes)
#     code = opcodes.get('mov', 0)
#     tmp = re.findall(r'/*[r,R][0-9]', str)
    
#    #res = ''
   
#   # for match in tmp:
#    #res = tmp.pop()
         
#     return res


def InsertOpcode(instrName:str):
    global countOperands
    if instrName == 'PUSH' or instrName == 'POP' or instrName == 'CALL':
        countOperands = 1
        
    opcode = opcodes[instrName]
    outStr = '__asm _emit 0x'
    byte = opcode.to_bytes(1, byteorder='big')
    outStr += byte.hex() + '\n'
    return outStr

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


def ParseInitVM(Buf:str):
    matches = re.findall(r'([_]{1}[0-9]{1,}[_][a-zA-Z]*)', Buf)
    arg1 = matches[0]
    arg2 = matches[1]

    outBuf = '_BEGIN_PROTECT_' + '(' + arg1 + ',' + arg2 + ')\n'
    outBuf += '__try {\n'

    cs = enigmaModule.init_crypto(None)
    outBuf += '__asm _emit 0x' + str(0xb0)
    return outBuf

def ParseEndVM(buf:str):
    outBuf = '}\n__except(Handler(GetExceptionInformation())){\n'
    outBuf += '_vm_destruct_(vm);\n}\n'


    return buf


def InsertOperandDefinition(szOp:str, type:int, isHex = False):
    outBuf = ''
    if type == optypes['reg'] or type == optypes['regaddr']:
        outBuf += 'define_operand(vm,' + str(type) + ',' + str(int(szOp) + 1) + ');\n'
    else:
        outBuf += 'define_operand(vm,' + str(type) + ','
        if hex:
            outBuf += '0x' + szOp + ');\n'
        else:
            outBuf += szOp + ');\n'

    return outBuf
    

def InsertOptype(buf:str):
    out = ''
    global szDefineOpsFunc
    op = re.findall(r'\[r([0-9]+)\]', buf)
    if len(op) != 0:
        type = optypes['regaddr']
        tmp = op[0]
        tmp = int(tmp)
        tmp += 1
        szDefineOpsFunc += InsertOperandDefinition(op[0], type)
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
        szDefineOpsFunc += InsertOperandDefinition(op[0], type)
        type <<= 4
        type |= tmp
        byte = type.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte.hex() + '\n'
        return out

    op = re.findall(r'\[0x([a-fA-F0-9]+)\]', buf)
    if len(op) != 0:
        type = optypes['constaddr']
        byte1 = type.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte1.hex() + '\n'
        value = op[0]
        szDefineOpsFunc += InsertOperandDefinition(value, type, True)
        szFirstByte = ''
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
        byte1 = type.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte1.hex() + '\n'
        value = op[0]
        szDefineOpsFunc += InsertOperandDefinition(value, type, False)
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
        szDefineOpsFunc += InsertOperandDefinition(op[0], type, True)
        byte = type.to_bytes(1, 'big')
        out += '__asm _emit 0x'
        out += byte.hex() + '\n'
        return out

    op = re.findall(r'([0-9a-fA-F]+)', buf)
    if len(op) != 0:
        type = optypes['const']
        szDefineOpsFunc += InsertOperandDefinition(op[0], type, False)
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

    
    

    return outBuf


def InsertInstruction(buf:str):
    global szDefineOpsFunc
    szDefineOpsFunc = ''
    outBuf = ''
    instruction = re.findall(r'/*(vm|VM)[_]{1}([A-Za-z]{2,})', buf)
   # i = len(instruction)
    spl = buf.rsplit('VM_', 1)
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


def main():
    #for file in sys.argv:
 #  Enigma.enigmaModule.init_crypto()
   # enigmaModule.Encrypt(POINTER(cs), c_ubyte('0')())
   # enigmaModule.func()
    
    
    ParseFile("../Virtualization/main.c")
    return 0
    



if __name__ == "__main__":
    main()
