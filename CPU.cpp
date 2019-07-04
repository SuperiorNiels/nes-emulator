//
// Created by niels on 7/2/19.
//

#include <iostream>
#include "CPU.h"

CPU::CPU() {
    PC = 0x0000;
    AC = 0x00;
    X = 0x00;
    Y = 0x00;
    SR = 0x00;
    SP = 0x00;

    // Intialize instructions
    for(auto & i : instructions) {
        i = instruction();
    }

    initialize_instructions(instructions);
    std::cout << "temp";
}
