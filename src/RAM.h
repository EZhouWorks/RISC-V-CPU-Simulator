//64 mB RAM
//Each block = 64Byte
//mapping notation: cacheline = Block Number mod 256
#ifndef RISC_V_CPU_SIMULATOR_RAM_H
#define RISC_V_CPU_SIMULATOR_RAM_H
#include <cstdint>
#include<vector>

class RAM {
public:
    vector<uint8_t> memory;

    RAM() { //initialize
        memory.resize(67108864);
        for (int i=0; i<memory.size(); i++) {
            memory[i] = 0;
        }
    }

    uint8_t readCell(uint32_t address) {
        return memory[address];
    }

    void writeCell(uint32_t address, uint8_t data) {
        memory[address] = data;
    }

};
#endif //RISC_V_CPU_SIMULATOR_RAM_H
