//
// Created by zhoue on 2026/7/17.
//

#ifndef RISC_V_CPU_SIMULATOR_PIPELINEREGISTERS_H
#define RISC_V_CPU_SIMULATOR_PIPELINEREGISTERS_H
#include <cstdint>
#include "Controller.h"
struct IF_ID_data {
    int valid;
    int drain;
    uint32_t machine_code;
};
struct ID_EX_data {
    int valid;
    int drain;
    ALU_op ALU_operation;
    ALU_source ALU_source1;
    ALU_source ALU_source2;
    uint32_t rs1_addr;
    uint32_t rs2_addr;
    uint32_t rd_addr;
    uint32_t rs1_val;
    uint32_t rs2_val;
    int32_t I_12bit_imm;
    uint32_t I_shamt_imm;
    uint32_t Store_imm;
    Store_op Store_op;
    Memory_op Memory_op;
    Memory_data_type Memory_data_type;
};
struct EX_MEM_data {
    int valid;
    int drain;
    uint32_t ALU_result;
    Memory_op Memory_op;
    Memory_data_type Memory_data_type;
    uint32_t Store_imm;
    Store_op Store_op;
    uint32_t rd_addr;
    uint32_t rs1_val;
    uint32_t rs2_val;
};
struct MEM_WB_data {
    int valid;
    int drain;
    uint32_t ALU_result;
    uint32_t Store_imm;
    Store_op Store_op;
    uint32_t rd_addr;
    uint32_t rs1_val;
    uint32_t rs2_val;
};

class PipelineRegisters {
public:
    IF_ID_data IF_ID_register;
    ID_EX_data ID_EX_register;
    EX_MEM_data EX_MEM_register;
    MEM_WB_data MEM_WB_register;
    PipelineRegisters() {
        // this->IF_ID_register = 0b0;
        // this->ID_EX_register.ALU_operation =  NONE_op;
        // this->ID_EX_register.ALU_source = rs2;
        // this->EX_MEM_register = 0b0;
        // this->MEM_WB_register = 0b0;
        IF_ID_register.valid = 0;
        ID_EX_register.valid = 0;
        EX_MEM_register.valid = 0;
        MEM_WB_register.valid = 0;
        EX_MEM_register.Memory_op = READBYTE;
        MEM_WB_register.Store_op = STOREBYTE;
    }
};
#endif //RISC_V_CPU_SIMULATOR_PIPELINEREGISTERS_H
