#ifndef RISC_V_CPU_SIMULATOR_ALU_H
#define RISC_V_CPU_SIMULATOR_ALU_H
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include "Register.h"

class ALU {
private:
    uint32_t ADD(uint32_t x1_value, uint32_t x2_value) { //command 0
        return x1_value+x2_value;
    }
    uint32_t SUB(uint32_t x1_value, uint32_t x2_value) { //command 1
        return x1_value-x2_value;
    }
    uint32_t AND(uint32_t x1_value, uint32_t x2_value) { //command 2
        return x1_value&x2_value;
    }
    uint32_t OR(uint32_t x1_value, uint32_t x2_value) { //command 3
        return x1_value|x2_value;
    }
    uint32_t XOR(uint32_t x1_value, uint32_t x2_value) { //command 4
        return x1_value^x2_value;
    }

    uint32_t SLL(uint32_t x1_value, uint32_t shift_amount) { //command 5
        return x1_value<<shift_amount;
    }

    uint32_t SRL(uint32_t x1_value, uint32_t shift_amount) { //unsign; command 6
        return x1_value >> shift_amount;
    }

    uint32_t SRA(uint32_t x1_value, uint32_t shift_amount) { //signed; command 7
        return (int32_t)x1_value >> shift_amount;
    }

    uint32_t SLT(uint32_t x1_value, uint32_t x2_value) { //command 8
        //signed
        if ((int32_t)x1_value<(int32_t)x2_value) {
            return 1;
        }
        else {
            return 0;
        }
    }

    uint32_t SLTU(uint32_t x1_value, uint32_t x2_value){//unsigned; command 9
        if (x1_value<x2_value) {
            return 1;
        }
        else {
            return 0;
        }
    }
public:
    uint32_t operate(int command, uint32_t x1_value, uint32_t x2_value, uint32_t shift_amount = 0) {
        switch (command) {
                case 0: ADD(x1_value, x2_value);
                break;
                case 1: SUB(x1_value, x2_value);
                break;
                case 2: AND(x1_value, x2_value);
                break;
                case 3: OR(x1_value, x2_value);
                break;
                case 4: XOR(x1_value, x2_value);
                break;
                case 5: SLL(x1_value, x2_value);
                break;
                case 6: SRL(x1_value, x2_value);
                break;
                case 7: SRA(x1_value, x2_value);
                break;
                case 8: SLT(x1_value, x2_value);
                break;
                case 9: SLTU(x1_value, x2_value);
                break;
            default:
                cout<<"Unexpected Command"<<endl;
                break;
        }
    }

};

#endif //RISC_V_CPU_SIMULATOR_ALU_H
