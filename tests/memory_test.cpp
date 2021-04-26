#include <gtest/gtest.h>
#include "Memory.h"

TEST(MEM_TESTS, MemoryInitReadWriteTest) {
    int64_t cycle_count = 0;
    Memory mem;
    
    {
        Memory mem(1024); // 1024 byte mem
        mem.reset();
    
        EXPECT_EQ(mem.getMemorySize(), 1024);
        EXPECT_EQ(mem.read(cycle_count, 512), 0x00);

        mem.write(cycle_count, 512, 0x45);
        EXPECT_EQ(mem.read(cycle_count, 512), 0x45);
        mem.write(cycle_count, 513, 0x46);
        EXPECT_EQ(mem.read(cycle_count, 513), 0x46);
        EXPECT_EQ(mem.read16(cycle_count, 512), 0x4645); // little endian
    }

    // Memory should be unset
    EXPECT_EQ(mem.getMemorySize(), 0);
    EXPECT_EQ(mem.read(cycle_count, 512), 0); // does not add to cycle count   

    // Cycles check
    EXPECT_EQ(cycle_count, 7);
}