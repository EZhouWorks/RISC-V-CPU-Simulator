//16kb L1 cache
//256 cache lines
#ifndef RISC_V_CPU_SIMULATOR_L1CACHE_H
#define RISC_V_CPU_SIMULATOR_L1CACHE_H
#include<cstdint>
#include "RAM.h"
#include "L2Cache.h"

constexpr int BLOCK_SIZE = 64;
constexpr int CACHE_LINES = 256;

class L1Cache {
public:
    CacheLine cachelines[CACHE_LINES];
    L1Cache() { //initialize
        for (int i = 0; i < CACHE_LINES; i++) {
            cachelines[i].valid = 0;
            cachelines[i].tag = 0;
            for (int j = 0; j < BLOCK_SIZE; j++) {
                cachelines[i].bytes[j] = 0;
            }
        }
    }

    uint32_t readFullData(uint32_t addr, L2Cache &l2cache, RAM &ram) { //loads data from ram if missed, can also be used to access data in L1Cache
        uint32_t block = addr/64;
        uint32_t index = block%256;
        uint32_t tag = block/256;
        uint32_t offset = addr%64;

        CacheLine& target_cacheLine = cachelines[index];
        if (target_cacheLine.valid == 1 and target_cacheLine.tag == tag) { //hit
            cout<<"hit L1"<<endl;
            return
                  (uint32_t)target_cacheLine.bytes[offset]
                | ((uint32_t)target_cacheLine.bytes[offset+1] << 8)
                | ((uint32_t)target_cacheLine.bytes[offset+2] << 16)
                | ((uint32_t)target_cacheLine.bytes[offset+3] << 24);
        }
        else { //miss, read and load from L2 Cache
            cout<<"miss L1"<<endl;
            target_cacheLine = l2cache.readFullData(addr,ram);
            cachelines[index] = target_cacheLine;
            cachelines[index].valid = 1;
            cachelines[index].tag = tag;

            return
                  (uint32_t)target_cacheLine.bytes[offset]
                | ((uint32_t)target_cacheLine.bytes[offset+1] << 8)
                | ((uint32_t)target_cacheLine.bytes[offset+2] << 16)
                | ((uint32_t)target_cacheLine.bytes[offset+3] << 24);
        }
    }

};
#endif //RISC_V_CPU_SIMULATOR_L1CACHE_H
