import argparse

parser = argparse.ArgumentParser(description="Convert text file to binary file.")
parser.add_argument('--file', required=True)

args = parser.parse_args()
file = open(args.file)
data = file.read()
file.close()

counter, i = 0, 0
to_write = bytearray()
while i < len(data):
    if data[i] == '\n' or data[i] == ' ':
        i += 1
        continue
    hexstring = str(data[i]) + str(data[i + 1])
    value = int(hexstring, 16)
    to_write.append(value)
    i += 2

file = open(args.file, "w+b")
file.write(to_write)
file.close()