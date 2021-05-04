#ifndef NES_BUS
#define NES_BUS


class Bus {
public:
    virtual void reset() = 0;
    virtual void openROM(const char* filename) = 0;
    virtual uint8_t read(int64_t& cycles, uint16_t addr) = 0;
    virtual void write(int64_t& cycles, uint16_t addr, uint8_t data) = 0;
    virtual ~Bus() {};

};

#endif //NES_BUS
