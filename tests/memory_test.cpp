#include <gtest/gtest.h>
#include "DirectMemory.h"


TEST(MEM_TESTS, DirectMemoryInitReadWriteTest) {
    int64_t cycle_count = 0;
    DirectMemory mem(1024);
    
    mem.reset();

    EXPECT_EQ(mem.getMemorySize(), 1024);
    EXPECT_EQ(mem.read(cycle_count, 512), 0x00);

    mem.write(cycle_count, 512, 0x45);
    EXPECT_EQ(mem.read(cycle_count, 512), 0x45);
    mem.write(cycle_count, 513, 0x46);
    EXPECT_EQ(mem.read(cycle_count, 513), 0x46);

    // Cycles check
    EXPECT_EQ(cycle_count, 5);
}