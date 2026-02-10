#!/usr/bin/env python3
"""Generate a small 6502 CPU smoke-test ROM.

The program starts at 0x0400 and writes results to 0x0200-0x0207.
"""

from pathlib import Path

ROM_PATH = Path(__file__).resolve().parents[1] / "data" / "roms" / "cpu_smoke.bin"
PROGRAM_START = 0x0400

# 6502 program bytes starting at 0x0400
PROGRAM = [
    0xA2, 0x04,             # LDX #$04
    0xA0, 0x02,             # LDY #$02
    0xA9, 0x00,             # LDA #$00
    0x8D, 0x00, 0x02,       # STA $0200
    0x8D, 0x01, 0x02,       # STA $0201
    0xA9, 0x00,             # LDA #$00
    0x85, 0x14,             # STA $0014
    0xA9, 0x03,             # LDA #$03
    0x85, 0x15,             # STA $0015
    0xA9, 0x04,             # LDA #$04
    0x85, 0x20,             # STA $0020
    0xA9, 0x03,             # LDA #$03
    0x85, 0x21,             # STA $0021
    0xA9, 0x7F,             # LDA #$7F
    0x8D, 0x00, 0x03,       # STA $0300
    0xA9, 0x80,             # LDA #$80
    0x8D, 0x06, 0x03,       # STA $0306
    0xA1, 0x10,             # LDA ($10,X)
    0x8D, 0x00, 0x02,       # STA $0200
    0xB1, 0x20,             # LDA ($20),Y
    0x8D, 0x01, 0x02,       # STA $0201
    0xA9, 0x50,             # LDA #$50
    0x69, 0x10,             # ADC #$10
    0x8D, 0x02, 0x02,       # STA $0202
    0xA9, 0x50,             # LDA #$50
    0x69, 0x50,             # ADC #$50
    0x8D, 0x03, 0x02,       # STA $0203
    0xA9, 0x10,             # LDA #$10
    0x38,                   # SEC
    0xE9, 0x01,             # SBC #$01
    0x8D, 0x04, 0x02,       # STA $0204
    0xA9, 0x00,             # LDA #$00
    0xF0, 0x02,             # BEQ +2
    0xA9, 0xFF,             # LDA #$FF (skipped)
    0x8D, 0x05, 0x02,       # STA $0205
    0x20, 0x5A, 0x04,       # JSR $045A
    0xA9, 0x01,             # LDA #$01
    0x8D, 0x06, 0x02,       # STA $0206
    0x4C, 0x60, 0x04,       # JMP $0460
    0xA9, 0xAA,             # LDA #$AA (subroutine at $045A)
    0x8D, 0x07, 0x02,       # STA $0207
    0x60,                   # RTS
    0x4C, 0x60, 0x04,       # JMP $0460 (done loop)
]

EXPECTED = {
    0x0200: 0x7F,  # LDA ($10,X)
    0x0201: 0x80,  # LDA ($20),Y
    0x0202: 0x60,  # ADC #$10
    0x0203: 0xA0,  # ADC #$50
    0x0204: 0x0F,  # SBC #$01
    0x0205: 0x00,  # BEQ path
    0x0206: 0x01,  # After JSR/RTS
    0x0207: 0xAA,  # Subroutine write
}


def main() -> None:
    rom = bytearray([0x00] * PROGRAM_START)
    rom.extend(PROGRAM)
    ROM_PATH.parent.mkdir(parents=True, exist_ok=True)
    ROM_PATH.write_bytes(rom)

    print(f"Wrote {ROM_PATH}")
    print("Expected memory results after execution:")
    for addr in sorted(EXPECTED.keys()):
        print(f"  ${addr:04X} = ${EXPECTED[addr]:02X}")


if __name__ == "__main__":
    main()
