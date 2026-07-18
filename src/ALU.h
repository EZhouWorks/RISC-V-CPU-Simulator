#ifndef RISC_V_CPU_SIMULATOR_ALU_H
#define RISC_V_CPU_SIMULATOR_ALU_H
#include <cstdint>

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
        return x1_value<<(shift_amount & 0x1f);
    }

    uint32_t SRL(uint32_t x1_value, uint32_t shift_amount) { //unsign; command 6
        return x1_value >> (shift_amount & 0x1f);
    }

    uint32_t SRA(uint32_t x1_value, uint32_t shift_amount) { //signed; command 7
        return (uint32_t)((int32_t)x1_value >> (shift_amount & 0x1f));
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
    uint32_t operate(int command, uint32_t x1_value = 0, uint32_t x2_value = 0, uint32_t shift_amount = 0) {
        switch (command) {
                case 0: return ADD(x1_value, x2_value);
                case 1: return SUB(x1_value, x2_value);
                case 2: return AND(x1_value, x2_value);
                case 3: return OR(x1_value, x2_value);
                case 4: return XOR(x1_value, x2_value);
                case 5: return SLL(x1_value, (shift_amount & 0x1f));
                case 6: return SRL(x1_value, (shift_amount & 0x1f));
                case 7: return SRA(x1_value, (shift_amount & 0x1f));
                case 8: return SLT(x1_value, x2_value);
                case 9: return SLTU(x1_value, x2_value);
                default:
                    cout<<"Unexpected Command"<<endl;
                    break;
        }
    }

};

#endif //RISC_V_CPU_SIMULATOR_ALU_H
