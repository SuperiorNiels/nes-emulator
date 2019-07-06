#include <iostream>
#include "Console.h"

int main() {
    Console c;
    c.loadROM("../cpu_dummy_reads.nes");
    c.run();
    return 0;
}