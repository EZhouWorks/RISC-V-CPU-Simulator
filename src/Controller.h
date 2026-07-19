//
// Created by zhoue on 2026/7/17.
//

#ifndef RISC_V_CPU_SIMULATOR_CONTROLLER_H
#define RISC_V_CPU_SIMULATOR_CONTROLLER_H
#include <cstdint>
#include "ALU.h"
enum ALU_source { //indicate 2nd num from rs2/imm
    rs2,
    imm
};

class Controller {
public:
    ALU_op ALU_operation;
    ALU_source ALU_source;
    uint32_t Register_operation = 0;
    uint32_t Memory_operation = 0;
    uint32_t DataPath_operation = 0;
    Controller(){};
    void SetControlSignal(uint32_t decoder_code) {
        switch (decoder_code) {
            //R type
            case(0): this->ALU_operation = NONE_op, ALU_source = rs2; break;
            case(1): this->ALU_operation = ADD_op; ALU_source = rs2; break;
            case(2): this->ALU_operation = SUB_op; ALU_source = rs2; break;
            case(3): this->ALU_operation = SLL_op; ALU_source = rs2; break;
            case(4): this->ALU_operation = SLT_op; ALU_source = rs2; break;
            case(5): this->ALU_operation = SLTU_op; ALU_source = rs2; break;
            case(6): this->ALU_operation = XOR_op; ALU_source = rs2; break;
            case(7): this->ALU_operation = SRL_op; ALU_source = rs2; break;
            case(8): this->ALU_operation = SRA_op; ALU_source = rs2; break;
            case(9): this->ALU_operation = OR_op; ALU_source = rs2; break;
            case(10): this->ALU_operation = AND_op; ALU_source = rs2; break;
            //I type ALU
            case(11): this->ALU_operation = ADD_op; ALU_source = imm; break;
            case(12): this->ALU_operation = SLT_op; ALU_source = imm; break;
            case(13): this->ALU_operation = SLTU_op; ALU_source = imm; break;
            case(14): this->ALU_operation = XOR_op; ALU_source = imm; break;
            case(15): this->ALU_operation = SLL_op; ALU_source = imm; break;
            case(16): this->ALU_operation = SRL_op; ALU_source = imm; break;
            case(17): this->ALU_operation = SRA_op; ALU_source = imm; break;
            case(18): this->ALU_operation = OR_op; ALU_source = imm; break;
            case(19): this->ALU_operation = AND_op; ALU_source = imm;; break;

        }
    }
};
#endif //RISC_V_CPU_SIMULATOR_CONTROLLER_H
