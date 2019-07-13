#include <iostream>
#include "Console.h"

int main() {
    Console c;
    c.loadROM("../test.bin");
    c.run();
    return 0;
}