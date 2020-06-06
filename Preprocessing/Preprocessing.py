import re
import sys


#--------------------------------------

instructionPattern = { 'VM_MOV', 
                       'VM_AND',
                       'VM_OR',
                       'VM_XOR',
                       'VM_ADD',
                       'VM_SUB' }



#--------------------------------------

#
#   Парсинг строки
#
def ParseLine(str):
    


#
#   Проверяет на наличие нужной строки
#
def FindInstruction(str, fdBuf):
    for instr in instructionPattern:
        if str.find(instr) != -1:
            return True
    return False          

#
#   Читает файл в буфер
#
def ParseFile(nameFile):
    fdFrom = open(nameFile, "r")
    fdIn = open(nameFile[:-2] + "_1.c", "w")
    fdBuf = ""
    tmp = ""

    try:
        for string in fdFrom:
            tmp = string
            #print(tmp)
            if FindInstruction(tmp, fdBuf) == True:
                 fdBuf += ParseLine(tmp)
            else:
                 fdBuf += tmp
    finally:
        fdIn.close()
        fdFrom.close()

    print(fdBuf)
    fdIn.close()
    fdFrom.close()




def main():
    #for file in sys.argv:
    ParseFile("../Virtualization/main.c")

    



if __name__ == "__main__":
    main()
