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

inline void initialize_instructions(instruction *instructions) {
    // Define instructions (auto generated)
    instructions[0].type = ADC;
    instructions[0].opcode = 0x69;
    instructions[0].mode = ADDR_IMM;
    instructions[1].type = ADC;
    instructions[1].opcode = 0x65;
    instructions[1].mode = ADDR_ZER;
    instructions[2].type = ADC;
    instructions[2].opcode = 0x75;
    instructions[2].mode = ADDR_ZEX;
    instructions[3].type = ADC;
    instructions[3].opcode = 0x6D;
    instructions[3].mode = ADDR_ABS;
    instructions[4].type = ADC;
    instructions[4].opcode = 0x7D;
    instructions[4].mode = ADDR_ABX;
    instructions[5].type = ADC;
    instructions[5].opcode = 0x79;
    instructions[5].mode = ADDR_ABY;
    instructions[6].type = ADC;
    instructions[6].opcode = 0x61;
    instructions[6].mode = ADDR_INX;
    instructions[7].type = ADC;
    instructions[7].opcode = 0x71;
    instructions[7].mode = ADDR_INY;
    instructions[8].type = AND;
    instructions[8].opcode = 0x29;
    instructions[8].mode = ADDR_IMM;
    instructions[9].type = AND;
    instructions[9].opcode = 0x25;
    instructions[9].mode = ADDR_ZER;
    instructions[10].type = AND;
    instructions[10].opcode = 0x35;
    instructions[10].mode = ADDR_ZEX;
    instructions[11].type = AND;
    instructions[11].opcode = 0x2D;
    instructions[11].mode = ADDR_ABS;
    instructions[12].type = AND;
    instructions[12].opcode = 0x3D;
    instructions[12].mode = ADDR_ABX;
    instructions[13].type = AND;
    instructions[13].opcode = 0x39;
    instructions[13].mode = ADDR_ABY;
    instructions[14].type = AND;
    instructions[14].opcode = 0x21;
    instructions[14].mode = ADDR_INX;
    instructions[15].type = AND;
    instructions[15].opcode = 0x31;
    instructions[15].mode = ADDR_INY;
    instructions[16].type = ASL;
    instructions[16].opcode = 0x0A;
    instructions[16].mode = ADDR_ACC;
    instructions[17].type = ASL;
    instructions[17].opcode = 0x06;
    instructions[17].mode = ADDR_ZER;
    instructions[18].type = ASL;
    instructions[18].opcode = 0x16;
    instructions[18].mode = ADDR_ZEX;
    instructions[19].type = ASL;
    instructions[19].opcode = 0x0E;
    instructions[19].mode = ADDR_ABS;
    instructions[20].type = ASL;
    instructions[20].opcode = 0x1E;
    instructions[20].mode = ADDR_ABX;
    instructions[21].type = BCC;
    instructions[21].opcode = 0x90;
    instructions[21].mode = ADDR_REL;
    instructions[22].type = BCS;
    instructions[22].opcode = 0xB0;
    instructions[22].mode = ADDR_REL;
    instructions[23].type = BEQ;
    instructions[23].opcode = 0xF0;
    instructions[23].mode = ADDR_REL;
    instructions[24].type = BIT;
    instructions[24].opcode = 0x24;
    instructions[24].mode = ADDR_ZER;
    instructions[25].type = BIT;
    instructions[25].opcode = 0x2C;
    instructions[25].mode = ADDR_ABS;
    instructions[26].type = BMI;
    instructions[26].opcode = 0x30;
    instructions[26].mode = ADDR_REL;
    instructions[27].type = BNE;
    instructions[27].opcode = 0xD0;
    instructions[27].mode = ADDR_REL;
    instructions[28].type = BPL;
    instructions[28].opcode = 0x10;
    instructions[28].mode = ADDR_REL;
    instructions[29].type = BRK;
    instructions[29].opcode = 0x00;
    instructions[29].mode = ADDR_IMP;
    instructions[30].type = BVC;
    instructions[30].opcode = 0x50;
    instructions[30].mode = ADDR_REL;
    instructions[31].type = BVS;
    instructions[31].opcode = 0x70;
    instructions[31].mode = ADDR_REL;
    instructions[32].type = CLC;
    instructions[32].opcode = 0x18;
    instructions[32].mode = ADDR_IMP;
    instructions[33].type = CLD;
    instructions[33].opcode = 0xD8;
    instructions[33].mode = ADDR_IMP;
    instructions[34].type = CLI;
    instructions[34].opcode = 0x58;
    instructions[34].mode = ADDR_IMP;
    instructions[35].type = CLV;
    instructions[35].opcode = 0xB8;
    instructions[35].mode = ADDR_IMP;
    instructions[36].type = CMP;
    instructions[36].opcode = 0xC9;
    instructions[36].mode = ADDR_IMM;
    instructions[37].type = CMP;
    instructions[37].opcode = 0xC5;
    instructions[37].mode = ADDR_ZER;
    instructions[38].type = CMP;
    instructions[38].opcode = 0xD5;
    instructions[38].mode = ADDR_ZEX;
    instructions[39].type = CMP;
    instructions[39].opcode = 0xCD;
    instructions[39].mode = ADDR_ABS;
    instructions[40].type = CMP;
    instructions[40].opcode = 0xDD;
    instructions[40].mode = ADDR_ABX;
    instructions[41].type = CMP;
    instructions[41].opcode = 0xD9;
    instructions[41].mode = ADDR_ABY;
    instructions[42].type = CMP;
    instructions[42].opcode = 0xC1;
    instructions[42].mode = ADDR_INX;
    instructions[43].type = CMP;
    instructions[43].opcode = 0xD1;
    instructions[43].mode = ADDR_INY;
    instructions[44].type = CPX;
    instructions[44].opcode = 0xE0;
    instructions[44].mode = ADDR_IMM;
    instructions[45].type = CPX;
    instructions[45].opcode = 0xE4;
    instructions[45].mode = ADDR_ZER;
    instructions[46].type = CPX;
    instructions[46].opcode = 0xEC;
    instructions[46].mode = ADDR_ABS;
    instructions[47].type = CPY;
    instructions[47].opcode = 0xC0;
    instructions[47].mode = ADDR_IMM;
    instructions[48].type = CPY;
    instructions[48].opcode = 0xC4;
    instructions[48].mode = ADDR_ZER;
    instructions[49].type = CPY;
    instructions[49].opcode = 0xCC;
    instructions[49].mode = ADDR_ABS;
    instructions[50].type = DEC;
    instructions[50].opcode = 0xC6;
    instructions[50].mode = ADDR_ZER;
    instructions[51].type = DEC;
    instructions[51].opcode = 0xD6;
    instructions[51].mode = ADDR_ZEX;
    instructions[52].type = DEC;
    instructions[52].opcode = 0xCE;
    instructions[52].mode = ADDR_ABS;
    instructions[53].type = DEC;
    instructions[53].opcode = 0xDE;
    instructions[53].mode = ADDR_ABX;
    instructions[54].type = DEX;
    instructions[54].opcode = 0xCA;
    instructions[54].mode = ADDR_IMP;
    instructions[55].type = DEY;
    instructions[55].opcode = 0x88;
    instructions[55].mode = ADDR_IMP;
    instructions[56].type = EOR;
    instructions[56].opcode = 0x49;
    instructions[56].mode = ADDR_IMM;
    instructions[57].type = EOR;
    instructions[57].opcode = 0x45;
    instructions[57].mode = ADDR_ZER;
    instructions[58].type = EOR;
    instructions[58].opcode = 0x55;
    instructions[58].mode = ADDR_ZEX;
    instructions[59].type = EOR;
    instructions[59].opcode = 0x4D;
    instructions[59].mode = ADDR_ABS;
    instructions[60].type = EOR;
    instructions[60].opcode = 0x5D;
    instructions[60].mode = ADDR_ABX;
    instructions[61].type = EOR;
    instructions[61].opcode = 0x59;
    instructions[61].mode = ADDR_ABY;
    instructions[62].type = EOR;
    instructions[62].opcode = 0x41;
    instructions[62].mode = ADDR_INX;
    instructions[63].type = EOR;
    instructions[63].opcode = 0x51;
    instructions[63].mode = ADDR_INY;
    instructions[64].type = INC;
    instructions[64].opcode = 0xE6;
    instructions[64].mode = ADDR_ZER;
    instructions[65].type = INC;
    instructions[65].opcode = 0xF6;
    instructions[65].mode = ADDR_ZEX;
    instructions[66].type = INC;
    instructions[66].opcode = 0xEE;
    instructions[66].mode = ADDR_ABS;
    instructions[67].type = INC;
    instructions[67].opcode = 0xFE;
    instructions[67].mode = ADDR_ABX;
    instructions[68].type = INX;
    instructions[68].opcode = 0xE8;
    instructions[68].mode = ADDR_IMP;
    instructions[69].type = INY;
    instructions[69].opcode = 0xC8;
    instructions[69].mode = ADDR_IMP;
    instructions[70].type = JMP;
    instructions[70].opcode = 0x4C;
    instructions[70].mode = ADDR_ABS;
    instructions[71].type = JMP;
    instructions[71].opcode = 0x6C;
    instructions[71].mode = ADDR_IND;
    instructions[72].type = JSR;
    instructions[72].opcode = 0x20;
    instructions[72].mode = ADDR_ABS;
    instructions[73].type = LDA;
    instructions[73].opcode = 0xA9;
    instructions[73].mode = ADDR_IMM;
    instructions[74].type = LDA;
    instructions[74].opcode = 0xA5;
    instructions[74].mode = ADDR_ZER;
    instructions[75].type = LDA;
    instructions[75].opcode = 0xB5;
    instructions[75].mode = ADDR_ZEX;
    instructions[76].type = LDA;
    instructions[76].opcode = 0xAD;
    instructions[76].mode = ADDR_ABS;
    instructions[77].type = LDA;
    instructions[77].opcode = 0xBD;
    instructions[77].mode = ADDR_ABX;
    instructions[78].type = LDA;
    instructions[78].opcode = 0xB9;
    instructions[78].mode = ADDR_ABY;
    instructions[79].type = LDA;
    instructions[79].opcode = 0xA1;
    instructions[79].mode = ADDR_INX;
    instructions[80].type = LDA;
    instructions[80].opcode = 0xB1;
    instructions[80].mode = ADDR_INY;
    instructions[81].type = LDX;
    instructions[81].opcode = 0xA2;
    instructions[81].mode = ADDR_IMM;
    instructions[82].type = LDX;
    instructions[82].opcode = 0xA6;
    instructions[82].mode = ADDR_ZER;
    instructions[83].type = LDX;
    instructions[83].opcode = 0xB6;
    instructions[83].mode = ADDR_ZEY;
    instructions[84].type = LDX;
    instructions[84].opcode = 0xAE;
    instructions[84].mode = ADDR_ABS;
    instructions[85].type = LDX;
    instructions[85].opcode = 0xBE;
    instructions[85].mode = ADDR_ABY;
    instructions[86].type = LDY;
    instructions[86].opcode = 0xA0;
    instructions[86].mode = ADDR_IMM;
    instructions[87].type = LDY;
    instructions[87].opcode = 0xA4;
    instructions[87].mode = ADDR_ZER;
    instructions[88].type = LDY;
    instructions[88].opcode = 0xB4;
    instructions[88].mode = ADDR_ZEX;
    instructions[89].type = LDY;
    instructions[89].opcode = 0xAC;
    instructions[89].mode = ADDR_ABS;
    instructions[90].type = LDY;
    instructions[90].opcode = 0xBC;
    instructions[90].mode = ADDR_ABX;
    instructions[91].type = LSR;
    instructions[91].opcode = 0x4A;
    instructions[91].mode = ADDR_ACC;
    instructions[92].type = LSR;
    instructions[92].opcode = 0x46;
    instructions[92].mode = ADDR_ZER;
    instructions[93].type = LSR;
    instructions[93].opcode = 0x56;
    instructions[93].mode = ADDR_ZEX;
    instructions[94].type = LSR;
    instructions[94].opcode = 0x4E;
    instructions[94].mode = ADDR_ABS;
    instructions[95].type = LSR;
    instructions[95].opcode = 0x5E;
    instructions[95].mode = ADDR_ABX;
    instructions[96].type = NOP;
    instructions[96].opcode = 0xEA;
    instructions[96].mode = ADDR_IMP;
    instructions[97].type = ORA;
    instructions[97].opcode = 0x09;
    instructions[97].mode = ADDR_IMM;
    instructions[98].type = ORA;
    instructions[98].opcode = 0x05;
    instructions[98].mode = ADDR_ZER;
    instructions[99].type = ORA;
    instructions[99].opcode = 0x15;
    instructions[99].mode = ADDR_ZEX;
    instructions[100].type = ORA;
    instructions[100].opcode = 0x0D;
    instructions[100].mode = ADDR_ABS;
    instructions[101].type = ORA;
    instructions[101].opcode = 0x1D;
    instructions[101].mode = ADDR_ABX;
    instructions[102].type = ORA;
    instructions[102].opcode = 0x19;
    instructions[102].mode = ADDR_ABY;
    instructions[103].type = ORA;
    instructions[103].opcode = 0x01;
    instructions[103].mode = ADDR_INX;
    instructions[104].type = ORA;
    instructions[104].opcode = 0x11;
    instructions[104].mode = ADDR_INY;
    instructions[105].type = PHA;
    instructions[105].opcode = 0x48;
    instructions[105].mode = ADDR_IMP;
    instructions[106].type = PHP;
    instructions[106].opcode = 0x08;
    instructions[106].mode = ADDR_IMP;
    instructions[107].type = PLA;
    instructions[107].opcode = 0x68;
    instructions[107].mode = ADDR_IMP;
    instructions[108].type = PLP;
    instructions[108].opcode = 0x28;
    instructions[108].mode = ADDR_IMP;
    instructions[109].type = ROL;
    instructions[109].opcode = 0x2A;
    instructions[109].mode = ADDR_ACC;
    instructions[110].type = ROL;
    instructions[110].opcode = 0x26;
    instructions[110].mode = ADDR_ZER;
    instructions[111].type = ROL;
    instructions[111].opcode = 0x36;
    instructions[111].mode = ADDR_ZEX;
    instructions[112].type = ROL;
    instructions[112].opcode = 0x2E;
    instructions[112].mode = ADDR_ABS;
    instructions[113].type = ROL;
    instructions[113].opcode = 0x3E;
    instructions[113].mode = ADDR_ABX;
    instructions[114].type = ROR;
    instructions[114].opcode = 0x6A;
    instructions[114].mode = ADDR_ACC;
    instructions[115].type = ROR;
    instructions[115].opcode = 0x66;
    instructions[115].mode = ADDR_ZER;
    instructions[116].type = ROR;
    instructions[116].opcode = 0x76;
    instructions[116].mode = ADDR_ZEX;
    instructions[117].type = ROR;
    instructions[117].opcode = 0x6E;
    instructions[117].mode = ADDR_ABS;
    instructions[118].type = ROR;
    instructions[118].opcode = 0x7E;
    instructions[118].mode = ADDR_ABX;
    instructions[119].type = RTI;
    instructions[119].opcode = 0x40;
    instructions[119].mode = ADDR_IMP;
    instructions[120].type = RTS;
    instructions[120].opcode = 0x60;
    instructions[120].mode = ADDR_IMP;
    instructions[121].type = SBC;
    instructions[121].opcode = 0xE9;
    instructions[121].mode = ADDR_IMM;
    instructions[122].type = SBC;
    instructions[122].opcode = 0xE5;
    instructions[122].mode = ADDR_ZER;
    instructions[123].type = SBC;
    instructions[123].opcode = 0xF5;
    instructions[123].mode = ADDR_ZEX;
    instructions[124].type = SBC;
    instructions[124].opcode = 0xED;
    instructions[124].mode = ADDR_ABS;
    instructions[125].type = SBC;
    instructions[125].opcode = 0xFD;
    instructions[125].mode = ADDR_ABX;
    instructions[126].type = SBC;
    instructions[126].opcode = 0xF9;
    instructions[126].mode = ADDR_ABY;
    instructions[127].type = SBC;
    instructions[127].opcode = 0xE1;
    instructions[127].mode = ADDR_INX;
    instructions[128].type = SBC;
    instructions[128].opcode = 0xF1;
    instructions[128].mode = ADDR_INY;
    instructions[129].type = SEC;
    instructions[129].opcode = 0x38;
    instructions[129].mode = ADDR_IMP;
    instructions[130].type = SED;
    instructions[130].opcode = 0xF8;
    instructions[130].mode = ADDR_IMP;
    instructions[131].type = SEI;
    instructions[131].opcode = 0x78;
    instructions[131].mode = ADDR_IMP;
    instructions[132].type = STA;
    instructions[132].opcode = 0x85;
    instructions[132].mode = ADDR_ZER;
    instructions[133].type = STA;
    instructions[133].opcode = 0x95;
    instructions[133].mode = ADDR_ZEX;
    instructions[134].type = STA;
    instructions[134].opcode = 0x8D;
    instructions[134].mode = ADDR_ABS;
    instructions[135].type = STA;
    instructions[135].opcode = 0x9D;
    instructions[135].mode = ADDR_ABX;
    instructions[136].type = STA;
    instructions[136].opcode = 0x99;
    instructions[136].mode = ADDR_ABY;
    instructions[137].type = STA;
    instructions[137].opcode = 0x81;
    instructions[137].mode = ADDR_INX;
    instructions[138].type = STA;
    instructions[138].opcode = 0x91;
    instructions[138].mode = ADDR_INY;
    instructions[139].type = STX;
    instructions[139].opcode = 0x86;
    instructions[139].mode = ADDR_ZER;
    instructions[140].type = STX;
    instructions[140].opcode = 0x96;
    instructions[140].mode = ADDR_ZEY;
    instructions[141].type = STX;
    instructions[141].opcode = 0x8E;
    instructions[141].mode = ADDR_ABS;
    instructions[142].type = STY;
    instructions[142].opcode = 0x84;
    instructions[142].mode = ADDR_ZER;
    instructions[143].type = STY;
    instructions[143].opcode = 0x94;
    instructions[143].mode = ADDR_ZEX;
    instructions[144].type = STY;
    instructions[144].opcode = 0x8C;
    instructions[144].mode = ADDR_ABS;
    instructions[145].type = TAX;
    instructions[145].opcode = 0xAA;
    instructions[145].mode = ADDR_IMP;
    instructions[146].type = TAY;
    instructions[146].opcode = 0xA8;
    instructions[146].mode = ADDR_IMP;
    instructions[147].type = TSX;
    instructions[147].opcode = 0xBA;
    instructions[147].mode = ADDR_IMP;
    instructions[148].type = TXA;
    instructions[148].opcode = 0x8A;
    instructions[148].mode = ADDR_IMP;
    instructions[149].type = TXS;
    instructions[149].opcode = 0x9A;
    instructions[149].mode = ADDR_IMP;
    instructions[150].type = TYA;
    instructions[150].opcode = 0x98;
    instructions[150].mode = ADDR_IMP;
}


#endif //NES_DEFINES_H
