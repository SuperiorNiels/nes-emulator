#include <iostream>
#include "Console.h"

int main() {
    Console c;
    c.loadROM("../program.hex");
    c.run();
    return 0;
}