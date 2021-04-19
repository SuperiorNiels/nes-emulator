import argparse
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument("-b", required=True)

args = parser.parse_args()

file_path = Path(args.b)
if not file_path.exists(): exit(-1)

fp = open(args.b, 'rb')

data = []
byte = fp.read(1)
data.append(byte)
while byte: 
    byte = fp.read(1)
    data.append(byte)

fp.close()

print("Appending %d '0x00' to fix the binary." % (65535 - len(data) + 2))

while len(data) < 65535 + 2: data.insert(0, b'\x00')

print("Fixed binary size: %d bytes" % len(data))

fp = open(args.b, 'wb')
for byte in data: fp.write(byte)
fp.close()