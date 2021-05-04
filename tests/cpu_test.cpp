#include <gtest/gtest.h>
#include "CPU.h"
#include "RAM_Bus.h"

TEST(CPU_TESTS, FullFunctionalTestNoDecimal) {
    CPU cpu;
    RAM_Bus bus;

    const char* bin_location = "../data/roms/functional_test_no_decimal_ops.bin";

    bus.openROM(bin_location);

    cpu.attachBus(&bus);
    cpu.setCPUSignal(RESET, true);
    cpu.execute(1); // cpu reset (put in known state)
    cpu.setProgramCounter(0x0400); // start of the loaded program
    cpu.execute(100000000);
    
    auto final_state = cpu.getCPUState();

    // Check if CPU reached program end (only possible when all tests succeeded)
    EXPECT_EQ(final_state.PC, 0x336d);
}