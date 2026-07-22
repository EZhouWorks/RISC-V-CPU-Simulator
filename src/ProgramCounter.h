//
// Created by zhoue on 2026/7/17.
//

#ifndef RISC_V_CPU_SIMULATOR_PROGRAMCOUNTER_H
#define RISC_V_CPU_SIMULATOR_PROGRAMCOUNTER_H
#include <cstdint>

class ProgramCounter {
public:
    uint32_t PC_value = 0;
    uint32_t End_value;
    ProgramCounter(uint32_t entry_point,uint32_t end_point) {
        this->PC_value = entry_point;
        this->End_value = end_point;
    }

    void StepForward(int step) {
        PC_value = PC_value + step;
    }
    int CheckValid() { //this checks if the fetched machine code is still within the valid range
        if (PC_value < End_value) {
            return 1;
        }
        else {
            return 0;
        }
    }
    

};
#endif //RISC_V_CPU_SIMULATOR_PROGRAMCOUNTER_H
