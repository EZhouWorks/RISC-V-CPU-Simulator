//
// Created by zhoue on 2026/7/17.
//

#ifndef RISC_V_CPU_SIMULATOR_CPUCORE_H
#define RISC_V_CPU_SIMULATOR_CPUCORE_H
#include<iostream>
#include <cstdint>
#include "ALU.h"
#include "Decoder.h"
#include "Controller.h"
#include "RegisterFile.h"
class CPUcore {
public:
    Controller controller = Controller();
    ALU alu = ALU();
    Decoder decoder = Decoder();
    RegisterFile registerFile = RegisterFile();
    int core_id;

    CPUcore(int core_id) {
        this->core_id = core_id;
    }

    void Step(uint32_t machine_code) {
        uint32_t decoded_code = decoder.Decode(machine_code); //decodes machine code for controller
        uint32_t rs1_addr = decoder.rs1;
        uint32_t rs2_addr = decoder.rs2;
        uint32_t rd_addr = decoder.rd;
        uint32_t rs1_val = registerFile.read(rs1_addr);
        uint32_t rs2_val = registerFile.read(rs2_addr);
        int32_t I_12bit_imm = decoder.I_12bit_imm;
        uint32_t I_shamt_imm = decoder.I_shamt_imm;


        controller.SetControlSignal(decoded_code); //update control signal
        ALU_op alu_op = controller.ALU_operation;
        ALU_source alu_source = controller.ALU_source;
        uint32_t ALU_result = 0b0;

        if (alu_source == ALU_source::rs2) {
            ALU_result = alu.operate(alu_op, rs1_val, rs2_val);
        }
        else if (alu_source == ALU_source::I_12bit_imm) {
            ALU_result = alu.operate(alu_op, rs1_val, I_12bit_imm);
        }
        else if (alu_source == ALU_source::I_shamt_imm) {
            ALU_result = alu.operate(alu_op, rs1_val,I_shamt_imm);
        }

        registerFile.write(rd_addr,ALU_result);//write back

    }

};

#endif //RISC_V_CPU_SIMULATOR_CPUCORE_H
