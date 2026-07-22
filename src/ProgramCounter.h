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

    int StepForward(int steps) {
        if (PC_value+steps <= End_value+4) {
            PC_value = PC_value + steps;
            cout<<"PC_VAL"<<PC_value<<endl;
            return 1;
        }
        else {
            cout<<"PC out of range"<<endl;
            return 0;
        }
    }
    

};
#endif //RISC_V_CPU_SIMULATOR_PROGRAMCOUNTER_H
