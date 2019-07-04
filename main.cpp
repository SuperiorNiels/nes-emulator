#include <iostream>
#include "Console.h"

int main() {
    Console c;
    c.loadROM("../test_program.bin");
    c.run();
    return 0;
}