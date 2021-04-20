import json, enum

file = open("output.json")
data = json.loads(file.read())

unique = []

for row in data.keys():
    type = data[row]['type']
    if type not in unique:
        unique.append(type)

print("# of types: %d" % (len(unique)))

print("typedef enum {")
for d in unique:
    print("\t" + d + ",")
print("} instr_type;")

class Mode(enum.Enum):
    ADDR_IMP = 1
    ADDR_ACC = 2
    ADDR_IMM = 3
    ADDR_ZER = 4
    ADDR_ZEX = 5
    ADDR_ZEY = 6
    ADDR_REL = 7
    ADDR_ABS = 8
    ADDR_ABX = 9
    ADDR_ABY = 10
    ADDR_IND = 11
    ADDR_INX = 12
    ADDR_INY = 13

count = 0
for row in data.keys():
    type = data[row]['type']
    mode = Mode(data[row]['mode'])
    op_code = row
    print("instructions[%d].type = %s;" % (count, type))
    print("instructions[%d].opcode = 0x%s;" % (count, op_code))
    print("instructions[%d].mode = %s;" %  (count, mode.name))
    count += 1

