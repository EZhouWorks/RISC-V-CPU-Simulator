//
// Created by zhoue on 2026/7/17.
//

#ifndef RISC_V_CPU_SIMULATOR_PROGRAMCOUNTER_H
#define RISC_V_CPU_SIMULATOR_PROGRAMCOUNTER_H
#include <cstdint>

class ProgramCounter {
public:
    uint32_t PC_value = 0;
    ProgramCounter(uint32_t entry_point) {
        this->PC_value = entry_point;
    }

    void StepForward(int steps) {
        PC_value = PC_value + steps;
    }
    

};
#endif //RISC_V_CPU_SIMULATOR_PROGRAMCOUNTER_H
