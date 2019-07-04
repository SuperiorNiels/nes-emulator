//
// Created by niels on 04.07.19.
//

#ifndef NES_DEFINES_H
#define NES_DEFINES_H

#include <cstdint>
#include <map>

typedef enum {
    ADC,
    AND,
    ASL,
    BCC,
    BCS,
    BEQ,
    BIT,
    BMI,
    BNE,
    BPL,
    BRK,
    BVC,
    BVS,
    CLC,
    CLD,
    CLI,
    CLV,
    CMP,
    CPX,
    CPY,
    DEC,
    DEX,
    DEY,
    EOR,
    INC,
    INX,
    INY,
    JMP,
    JSR,
    LDA,
    LDX,
    LDY,
    LSR,
    NOP,
    ORA,
    PHA,
    PHP,
    PLA,
    PLP,
    ROL,
    ROR,
    RTI,
    RTS,
    SBC,
    SEC,
    SED,
    SEI,
    STA,
    STX,
    STY,
    TAX,
    TAY,
    TSX,
    TXA,
    TXS,
    TYA
} instr_type;

typedef enum {
    ADDR_IMP = 1,
    ADDR_ACC = 2,
    ADDR_IMM = 3,
    ADDR_ZER = 4,
    ADDR_ZEX = 5,
    ADDR_ZEY = 6,
    ADDR_REL = 7,
    ADDR_ABS = 8,
    ADDR_ABX = 9,
    ADDR_ABY = 10,
    ADDR_IND = 11,
    ADDR_INX = 12,
    ADDR_INY = 13,
} addr_mode;

typedef struct {
    addr_mode mode;
    instr_type type;
} instruction;

typedef enum {
    CARRY,
    ZERO,
    INTERRUPT,
    DECIMAL_MODE,
    BREAK_COMMAND,
    OVERFLOW,
    NEGATIVE,
} status_flags;

inline void initialize_instructions(std::map<uint8_t, instruction>*instructions) {
    instruction instr;
    instr.type = ADC;
    instr.mode = ADDR_IMM;
    instructions->emplace(0x69, instr);
    instr.type = ADC;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x65, instr);
    instr.type = ADC;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x75, instr);
    instr.type = ADC;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x6D, instr);
    instr.type = ADC;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x7D, instr);
    instr.type = ADC;
    instr.mode = ADDR_ABY;
    instructions->emplace(0x79, instr);
    instr.type = ADC;
    instr.mode = ADDR_INX;
    instructions->emplace(0x61, instr);
    instr.type = ADC;
    instr.mode = ADDR_INY;
    instructions->emplace(0x71, instr);
    instr.type = AND;
    instr.mode = ADDR_IMM;
    instructions->emplace(0x29, instr);
    instr.type = AND;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x25, instr);
    instr.type = AND;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x35, instr);
    instr.type = AND;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x2D, instr);
    instr.type = AND;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x3D, instr);
    instr.type = AND;
    instr.mode = ADDR_ABY;
    instructions->emplace(0x39, instr);
    instr.type = AND;
    instr.mode = ADDR_INX;
    instructions->emplace(0x21, instr);
    instr.type = AND;
    instr.mode = ADDR_INY;
    instructions->emplace(0x31, instr);
    instr.type = ASL;
    instr.mode = ADDR_ACC;
    instructions->emplace(0x0A, instr);
    instr.type = ASL;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x06, instr);
    instr.type = ASL;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x16, instr);
    instr.type = ASL;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x0E, instr);
    instr.type = ASL;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x1E, instr);
    instr.type = BCC;
    instr.mode = ADDR_REL;
    instructions->emplace(0x90, instr);
    instr.type = BCS;
    instr.mode = ADDR_REL;
    instructions->emplace(0xB0, instr);
    instr.type = BEQ;
    instr.mode = ADDR_REL;
    instructions->emplace(0xF0, instr);
    instr.type = BIT;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x24, instr);
    instr.type = BIT;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x2C, instr);
    instr.type = BMI;
    instr.mode = ADDR_REL;
    instructions->emplace(0x30, instr);
    instr.type = BNE;
    instr.mode = ADDR_REL;
    instructions->emplace(0xD0, instr);
    instr.type = BPL;
    instr.mode = ADDR_REL;
    instructions->emplace(0x10, instr);
    instr.type = BRK;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x00, instr);
    instr.type = BVC;
    instr.mode = ADDR_REL;
    instructions->emplace(0x50, instr);
    instr.type = BVS;
    instr.mode = ADDR_REL;
    instructions->emplace(0x70, instr);
    instr.type = CLC;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x18, instr);
    instr.type = CLD;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xD8, instr);
    instr.type = CLI;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x58, instr);
    instr.type = CLV;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xB8, instr);
    instr.type = CMP;
    instr.mode = ADDR_IMM;
    instructions->emplace(0xC9, instr);
    instr.type = CMP;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xC5, instr);
    instr.type = CMP;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0xD5, instr);
    instr.type = CMP;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xCD, instr);
    instr.type = CMP;
    instr.mode = ADDR_ABX;
    instructions->emplace(0xDD, instr);
    instr.type = CMP;
    instr.mode = ADDR_ABY;
    instructions->emplace(0xD9, instr);
    instr.type = CMP;
    instr.mode = ADDR_INX;
    instructions->emplace(0xC1, instr);
    instr.type = CMP;
    instr.mode = ADDR_INY;
    instructions->emplace(0xD1, instr);
    instr.type = CPX;
    instr.mode = ADDR_IMM;
    instructions->emplace(0xE0, instr);
    instr.type = CPX;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xE4, instr);
    instr.type = CPX;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xEC, instr);
    instr.type = CPY;
    instr.mode = ADDR_IMM;
    instructions->emplace(0xC0, instr);
    instr.type = CPY;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xC4, instr);
    instr.type = CPY;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xCC, instr);
    instr.type = DEC;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xC6, instr);
    instr.type = DEC;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0xD6, instr);
    instr.type = DEC;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xCE, instr);
    instr.type = DEC;
    instr.mode = ADDR_ABX;
    instructions->emplace(0xDE, instr);
    instr.type = DEX;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xCA, instr);
    instr.type = DEY;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x88, instr);
    instr.type = EOR;
    instr.mode = ADDR_IMM;
    instructions->emplace(0x49, instr);
    instr.type = EOR;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x45, instr);
    instr.type = EOR;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x55, instr);
    instr.type = EOR;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x4D, instr);
    instr.type = EOR;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x5D, instr);
    instr.type = EOR;
    instr.mode = ADDR_ABY;
    instructions->emplace(0x59, instr);
    instr.type = EOR;
    instr.mode = ADDR_INX;
    instructions->emplace(0x41, instr);
    instr.type = EOR;
    instr.mode = ADDR_INY;
    instructions->emplace(0x51, instr);
    instr.type = INC;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xE6, instr);
    instr.type = INC;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0xF6, instr);
    instr.type = INC;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xEE, instr);
    instr.type = INC;
    instr.mode = ADDR_ABX;
    instructions->emplace(0xFE, instr);
    instr.type = INX;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xE8, instr);
    instr.type = INY;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xC8, instr);
    instr.type = JMP;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x4C, instr);
    instr.type = JMP;
    instr.mode = ADDR_IND;
    instructions->emplace(0x6C, instr);
    instr.type = JSR;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x20, instr);
    instr.type = LDA;
    instr.mode = ADDR_IMM;
    instructions->emplace(0xA9, instr);
    instr.type = LDA;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xA5, instr);
    instr.type = LDA;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0xB5, instr);
    instr.type = LDA;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xAD, instr);
    instr.type = LDA;
    instr.mode = ADDR_ABX;
    instructions->emplace(0xBD, instr);
    instr.type = LDA;
    instr.mode = ADDR_ABY;
    instructions->emplace(0xB9, instr);
    instr.type = LDA;
    instr.mode = ADDR_INX;
    instructions->emplace(0xA1, instr);
    instr.type = LDA;
    instr.mode = ADDR_INY;
    instructions->emplace(0xB1, instr);
    instr.type = LDX;
    instr.mode = ADDR_IMM;
    instructions->emplace(0xA2, instr);
    instr.type = LDX;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xA6, instr);
    instr.type = LDX;
    instr.mode = ADDR_ZEY;
    instructions->emplace(0xB6, instr);
    instr.type = LDX;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xAE, instr);
    instr.type = LDX;
    instr.mode = ADDR_ABY;
    instructions->emplace(0xBE, instr);
    instr.type = LDY;
    instr.mode = ADDR_IMM;
    instructions->emplace(0xA0, instr);
    instr.type = LDY;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xA4, instr);
    instr.type = LDY;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0xB4, instr);
    instr.type = LDY;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xAC, instr);
    instr.type = LDY;
    instr.mode = ADDR_ABX;
    instructions->emplace(0xBC, instr);
    instr.type = LSR;
    instr.mode = ADDR_ACC;
    instructions->emplace(0x4A, instr);
    instr.type = LSR;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x46, instr);
    instr.type = LSR;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x56, instr);
    instr.type = LSR;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x4E, instr);
    instr.type = LSR;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x5E, instr);
    instr.type = NOP;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xEA, instr);
    instr.type = ORA;
    instr.mode = ADDR_IMM;
    instructions->emplace(0x09, instr);
    instr.type = ORA;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x05, instr);
    instr.type = ORA;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x15, instr);
    instr.type = ORA;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x0D, instr);
    instr.type = ORA;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x1D, instr);
    instr.type = ORA;
    instr.mode = ADDR_ABY;
    instructions->emplace(0x19, instr);
    instr.type = ORA;
    instr.mode = ADDR_INX;
    instructions->emplace(0x01, instr);
    instr.type = ORA;
    instr.mode = ADDR_INY;
    instructions->emplace(0x11, instr);
    instr.type = PHA;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x48, instr);
    instr.type = PHP;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x08, instr);
    instr.type = PLA;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x68, instr);
    instr.type = PLP;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x28, instr);
    instr.type = ROL;
    instr.mode = ADDR_ACC;
    instructions->emplace(0x2A, instr);
    instr.type = ROL;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x26, instr);
    instr.type = ROL;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x36, instr);
    instr.type = ROL;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x2E, instr);
    instr.type = ROL;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x3E, instr);
    instr.type = ROR;
    instr.mode = ADDR_ACC;
    instructions->emplace(0x6A, instr);
    instr.type = ROR;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x66, instr);
    instr.type = ROR;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x76, instr);
    instr.type = ROR;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x6E, instr);
    instr.type = ROR;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x7E, instr);
    instr.type = RTI;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x40, instr);
    instr.type = RTS;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x60, instr);
    instr.type = SBC;
    instr.mode = ADDR_IMM;
    instructions->emplace(0xE9, instr);
    instr.type = SBC;
    instr.mode = ADDR_ZER;
    instructions->emplace(0xE5, instr);
    instr.type = SBC;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0xF5, instr);
    instr.type = SBC;
    instr.mode = ADDR_ABS;
    instructions->emplace(0xED, instr);
    instr.type = SBC;
    instr.mode = ADDR_ABX;
    instructions->emplace(0xFD, instr);
    instr.type = SBC;
    instr.mode = ADDR_ABY;
    instructions->emplace(0xF9, instr);
    instr.type = SBC;
    instr.mode = ADDR_INX;
    instructions->emplace(0xE1, instr);
    instr.type = SBC;
    instr.mode = ADDR_INY;
    instructions->emplace(0xF1, instr);
    instr.type = SEC;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x38, instr);
    instr.type = SED;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xF8, instr);
    instr.type = SEI;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x78, instr);
    instr.type = STA;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x85, instr);
    instr.type = STA;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x95, instr);
    instr.type = STA;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x8D, instr);
    instr.type = STA;
    instr.mode = ADDR_ABX;
    instructions->emplace(0x9D, instr);
    instr.type = STA;
    instr.mode = ADDR_ABY;
    instructions->emplace(0x99, instr);
    instr.type = STA;
    instr.mode = ADDR_INX;
    instructions->emplace(0x81, instr);
    instr.type = STA;
    instr.mode = ADDR_INY;
    instructions->emplace(0x91, instr);
    instr.type = STX;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x86, instr);
    instr.type = STX;
    instr.mode = ADDR_ZEY;
    instructions->emplace(0x96, instr);
    instr.type = STX;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x8E, instr);
    instr.type = STY;
    instr.mode = ADDR_ZER;
    instructions->emplace(0x84, instr);
    instr.type = STY;
    instr.mode = ADDR_ZEX;
    instructions->emplace(0x94, instr);
    instr.type = STY;
    instr.mode = ADDR_ABS;
    instructions->emplace(0x8C, instr);
    instr.type = TAX;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xAA, instr);
    instr.type = TAY;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xA8, instr);
    instr.type = TSX;
    instr.mode = ADDR_IMP;
    instructions->emplace(0xBA, instr);
    instr.type = TXA;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x8A, instr);
    instr.type = TXS;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x9A, instr);
    instr.type = TYA;
    instr.mode = ADDR_IMP;
    instructions->emplace(0x98, instr);
}


#endif //NES_DEFINES_H
