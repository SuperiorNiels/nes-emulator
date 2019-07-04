//
// Created by niels on 04.07.19.
//

#ifndef NES_DEFINES_H
#define NES_DEFINES_H

#include <cstdint>

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
    uint8_t opcode;
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


#endif //NES_DEFINES_H
