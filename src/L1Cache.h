//16kb L1 cache
//256 cache lines
#ifndef RISC_V_CPU_SIMULATOR_L1CACHE_H
#define RISC_V_CPU_SIMULATOR_L1CACHE_H
#include<cstdint>
#include "RAM.h"

constexpr int BLOCK_SIZE = 64;
constexpr int CACHE_LINES = 256;

struct CacheLine {
    int valid;
    int tag;
    uint8_t byte[BLOCK_SIZE]; //each cacheline stores 16 commands， 64byte storage,
};

class L1Cache {
public:
    CacheLine cachelines[CACHE_LINES];
    L1Cache() { //initialize
        for (int i = 0; i < CACHE_LINES; i++) {
            cachelines[i].valid = 0;
            cachelines[i].tag = 0;
            for (int j = 0; j < BLOCK_SIZE; j++) {
                cachelines[i].byte[j] = 0;
            }
        }
    }

    void loadCacheBlockFromRAM(uint32_t addr, RAM &ram) {
        uint32_t block = addr/BLOCK_SIZE;
        uint32_t index = block%CACHE_LINES;
        uint32_t tag = block/CACHE_LINES;
        // uint32_t offset = addr%64;

        uint32_t blockStartAddr = block*BLOCK_SIZE;
        uint32_t blockEndAddr = blockStartAddr+BLOCK_SIZE-1;

        for (int i = blockStartAddr; i <= blockEndAddr; i++) {
            cachelines[index].byte[i-blockStartAddr] = ram.readCell(i);
        }

        cachelines[index].valid = 1;
        cachelines[index].tag = tag;
    }

    uint32_t readFullCommand(uint32_t addr, RAM &ram) { //loads data from ram if missed
        uint32_t block = addr/64;
        uint32_t index = block%256;
        uint32_t tag = block/256;
        uint32_t offset = addr%64;

        CacheLine& target_cacheLine = cachelines[index];
        if (target_cacheLine.valid == 1 and target_cacheLine.tag == tag) { //hit
            cout<<"hit"<<endl;
            return
                  (uint32_t)target_cacheLine.byte[offset]
                | ((uint32_t)target_cacheLine.byte[offset+1] << 8)
                | ((uint32_t)target_cacheLine.byte[offset+2] << 16)
                | ((uint32_t)target_cacheLine.byte[offset+3] << 24);
        }
        else { //miss, manually read from RAM
            cout<<"miss"<<endl;
            loadCacheBlockFromRAM(addr, ram);
            return 0b11111111111111111111111111111111;
        }
    }

};
#endif //RISC_V_CPU_SIMULATOR_L1CACHE_H
