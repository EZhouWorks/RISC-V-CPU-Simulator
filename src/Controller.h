//
// Created by zhoue on 2026/7/17.
//

#ifndef RISC_V_CPU_SIMULATOR_CONTROLLER_H
#define RISC_V_CPU_SIMULATOR_CONTROLLER_H
#include <cstdint>
#include "ALU.h"
#include "L1Cache.h"

class Controller {
public:
    ALU_op ALU_operation;
    ALU_source ALU_source1;
    ALU_source ALU_source2;
    Memory_op Memory_op;
    Memory_data_type Memory_data_type;
    Store_op Store_op;
    uint32_t Register_operation = 0;
    uint32_t Memory_operation = 0;
    Controller(){};
    void SetControlSignal(uint32_t decoder_code) {
        switch (decoder_code) {
            //R type
            case(0): this->ALU_operation = NO_ALU_OP; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(1): this->ALU_operation = ADD_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(2): this->ALU_operation = SUB_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(3): this->ALU_operation = SLL_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(4): this->ALU_operation = SLT_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(5): this->ALU_operation = SLTU_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(6): this->ALU_operation = XOR_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(7): this->ALU_operation = SRL_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(8): this->ALU_operation = SRA_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(9): this->ALU_operation = OR_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(10): this->ALU_operation = AND_op; ALU_source1 = rs1; ALU_source2 = rs2; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            //I type ALU
            case(11): this->ALU_operation = ADD_op; ALU_source1 = rs1; ALU_source2 = I_12bit_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(12): this->ALU_operation = SLT_op; ALU_source1 = rs1; ALU_source2 = I_12bit_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(13): this->ALU_operation = SLTU_op; ALU_source1 = rs1; ALU_source2 = I_12bit_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(14): this->ALU_operation = XOR_op; ALU_source1 = rs1; ALU_source2 = I_12bit_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(15): this->ALU_operation = SLL_op; ALU_source1 = rs1; ALU_source2 = I_shamt_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(16): this->ALU_operation = SRL_op; ALU_source1 = rs1; ALU_source2 = I_shamt_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(17): this->ALU_operation = SRA_op; ALU_source1 = rs1; ALU_source2 = I_shamt_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(18): this->ALU_operation = OR_op; ALU_source1 = rs1; ALU_source2 = I_12bit_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            case(19): this->ALU_operation = AND_op; ALU_source1 = rs1; ALU_source2 = I_12bit_imm; Memory_op = NO_MEMORY_OP; Store_op = NO_STORE_OP; break;
            //Load
            case(20): this->Memory_operation = READBYTE; ALU_source1 = rs2; ALU_source2 = I_12bit_imm; Memory_data_type = UNSIGN; Store_op = NO_STORE_OP;break;
            case(21): this->Memory_operation = READHALF; ALU_source1 = rs2; ALU_source2 = I_12bit_imm; Memory_data_type = UNSIGN; Store_op = NO_STORE_OP;break;
            case(22): this->Memory_operation = READWORD; ALU_source1 = rs2; ALU_source2 = I_12bit_imm; Memory_data_type = UNSIGN, Store_op = NO_STORE_OP;break;
            case(23): this->Memory_operation = READBYTE; ALU_source1 = rs2; ALU_source2 = I_12bit_imm; Memory_data_type = SIGN, Store_op = NO_STORE_OP;break;
            case(24): this->Memory_operation = READHALF; ALU_source1 = rs2; ALU_source2 = I_12bit_imm; Memory_data_type = SIGN, Store_op = NO_STORE_OP;break;
            //Store
            case(25): this->Store_op = STOREBYTE; ALU_source1 = rs1; ALU_source2 = store_imm; Memory_op = NO_MEMORY_OP;break;
            case(26): this->Store_op = STOREHALF; ALU_source1 = rs1; ALU_source2 = store_imm; Memory_op = NO_MEMORY_OP;break;
            case(27): this->Store_op = STOREWORD; ALU_source1 = rs1; ALU_source2 = store_imm; Memory_op = NO_MEMORY_OP;break;

        }
    }
};
#endif //RISC_V_CPU_SIMULATOR_CONTROLLER_H
