#include <gtest/gtest.h>
#include "CPU.h"
#include "Memory.h"

TEST(CPU_TESTS, FullFunctionalTestNoDecimal) {
    CPU cpu;
    Memory mem;

    const char* bin_location = "..\\tests\\data\\functional_test_no_decimal_ops.bin";

    mem.reset();
    mem.loadBinary(bin_location);

    cpu.attachMemeory(&mem);
    cpu.setResetVector(0x0400); // start of the loaded program
    cpu.setCPUSignal(RESET, true);
    cpu.execute(100000000);
    
    auto final_state = cpu.getCPUState();

    // Check if CPU reached program end (only possible when all tests succeeded)
    EXPECT_EQ(final_state.PC, 0x336d);
}