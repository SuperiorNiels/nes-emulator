import json, enum

from bs4 import BeautifulSoup

class Mode(enum.Enum):
    IMP = 1
    ACC = 2
    IMM = 3
    ZER = 4
    ZEX = 5
    ZEY = 6
    REL = 7
    ABS = 8
    ABX = 9
    ABY = 10
    IND = 11
    INX = 12
    INY = 13

modes = {
    "Immediate":    Mode.IMM,
    "Zero Page":    Mode.ZER,
    "Zero Page,X":  Mode.ZEX,
    "Absolute":     Mode.ABS,
    "Absolute,X":   Mode.ABX,
    "Absolute,Y":   Mode.ABY,
    "(Indirect,X)": Mode.INX,
    "(Indirect),Y": Mode.INY,
    "Relative":     Mode.REL,
    "Implied":      Mode.IMP,
    "Accumulator":  Mode.ACC,
    "Indirect":     Mode.IND,
    "Zero Page, Y": Mode.ZEY
}

def check_addr_mode(addr_mode):
    test = addr_mode.lower().replace(" ", "").replace('\n', "").strip()
    for key in modes.keys():
        if(test == key.lower().replace(" ", "")):
            return modes[key]
    return False

instructions = open("output.json", "r")
instr = json.loads(instructions.read())
instructions.close()
print("Loaded %d instructions." % (len(instr)))
print("Last loaded instructions: %s" % (instr[list(instr.keys())[-1]]["type"]))

type = input("Pleace type the type of command: ")

file = open("./input")
data = file.read()
file.close()

table_data = [[cell.text for cell in row("td")] for row in BeautifulSoup(data, "lxml")("tr")]


for r in table_data[1:]:
    mode = check_addr_mode(r[0])
    if(mode):
        op_code = r[1][2:]
        bytes = int(r[2])
        cycles = int(r[3][0])
        print(type + ", " + str(mode.value) + ", " + op_code + ", " + str(bytes) + ", " + str(cycles))
        instr[op_code] = {
            'type': type,
            'mode': mode.value,
            'bytes': bytes,
            'cyles': cycles
        }
    else:
        print("Addressing mode: %s not found!" % (r[0]))

verify = input("Is the file parsed correctly [Y/n]? ")
if(verify.lower() != "n"):
    print("Appending to output.json")
    output = open("output.json", "w")
    output.write(json.dumps(instr))
    output.close()
else:
    print("Aborted!")
