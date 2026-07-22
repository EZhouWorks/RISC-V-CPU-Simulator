//
// Created by zhoue on 2026/7/17.
//

#ifndef RISC_V_CPU_SIMULATOR_CPUCORE_H
#define RISC_V_CPU_SIMULATOR_CPUCORE_H
#include <iostream>
#include <cstdint>
#include "ALU.h"
#include "Decoder.h"
#include "Controller.h"
#include "RegisterFile.h"
#include "L1Cache.h"
#include "RAM.h"
#include "PipelineRegisters.h"
#include "ProgramCounter.h"

class CPUcore {
public:
    Controller controller = Controller();
    ALU alu = ALU();
    Decoder decoder = Decoder();
    RegisterFile registerFile = RegisterFile();
    int core_id;
    L1Cache l1_cache = L1Cache();
    PipelineRegisters pipeline_registers = PipelineRegisters();
    ProgramCounter program_counter = ProgramCounter(0b0,255);

    CPUcore(int core_id) {
        this->core_id = core_id;
    }

    uint8_t CPULoadByte(uint32_t addr,L2Cache& l2cache,RAM& ram) {
        return l1_cache.readByte(addr,l2cache,ram);
    }
    uint16_t CPULoadHalfWord(uint32_t addr,L2Cache& l2cache,RAM& ram) {
        return l1_cache.readHalfWord(addr,l2cache,ram);
    }
    uint32_t CPULoadWord(uint32_t addr,L2Cache& l2cache,RAM& ram) {
        return l1_cache.readWord(addr,l2cache,ram);
    }

    void Fetch(ProgramCounter &program_counter,L2Cache& l2cache,RAM& ram) {
        pipeline_registers.IF_ID_register.machine_code = CPULoadWord(program_counter.PC_value,l2cache,ram);
        if (program_counter.StepForward(4) == 1) { //check if next step is valid
            pipeline_registers.IF_ID_register.valid = 1;
            pipeline_registers.IF_ID_register.drain = 4;
        }
        else {
            pipeline_registers.IF_ID_register.valid = 0;
            pipeline_registers.IF_ID_register.drain -= 1;
        }
    }
    void Decode(uint32_t machine_code) {
        if (pipeline_registers.IF_ID_register.valid == 1 or pipeline_registers.IF_ID_register.drain>=3) {
            controller.SetControlSignal(decoder.Decode(machine_code)); //decode and generate control singal
            //store data and control signal into pipeline register
            pipeline_registers.ID_EX_register.ALU_operation = controller.ALU_operation;
            pipeline_registers.ID_EX_register.ALU_source1 = controller.ALU_source1;
            pipeline_registers.ID_EX_register.ALU_source2 = controller.ALU_source2;
            pipeline_registers.ID_EX_register.Memory_op = controller.Memory_op;
            pipeline_registers.ID_EX_register.Memory_data_type = controller.Memory_data_type;

            pipeline_registers.ID_EX_register.rs1_addr = decoder.rs1;
            pipeline_registers.ID_EX_register.rs2_addr = decoder.rs2;
            pipeline_registers.ID_EX_register.rd_addr = decoder.rd;
            pipeline_registers.ID_EX_register.rs1_val = registerFile.read(decoder.rs1);
            pipeline_registers.ID_EX_register.rs2_val = registerFile.read(decoder.rs2);
            pipeline_registers.ID_EX_register.I_12bit_imm = decoder.I_12bit_imm;
            pipeline_registers.ID_EX_register.I_shamt_imm = decoder.I_shamt_imm;
            pipeline_registers.ID_EX_register.Memory_op = controller.Memory_op;
            pipeline_registers.ID_EX_register.Store_imm = decoder.Store_imm;
            pipeline_registers.ID_EX_register.Store_op = controller.Store_op;
            pipeline_registers.ID_EX_register.valid = 1;
        }
        else {
            pipeline_registers.ID_EX_register.valid = 0;
            pipeline_registers.ID_EX_register.drain = pipeline_registers.IF_ID_register.drain;
        }
    }
    void Execution() {
        if (pipeline_registers.ID_EX_register.valid == 1 or pipeline_registers.ID_EX_register.drain>=2) {
            uint32_t ALU_result = 0b0;
            ALU_op alu_op = pipeline_registers.ID_EX_register.ALU_operation;
            ALU_source alu_source1 = pipeline_registers.ID_EX_register.ALU_source1;
            ALU_source alu_source2 = pipeline_registers.ID_EX_register.ALU_source2;
            uint32_t rs1_val = pipeline_registers.ID_EX_register.rs1_val;
            uint32_t rs2_val = pipeline_registers.ID_EX_register.rs2_val;
            uint32_t I_12bit_imm = pipeline_registers.ID_EX_register.I_12bit_imm;
            uint32_t I_shamt_imm = pipeline_registers.ID_EX_register.I_shamt_imm;
            uint32_t store_imm = pipeline_registers.ID_EX_register.Store_imm;

            switch (alu_source1) {
                case rs1:
                    switch (alu_source2) {
                        case(ALU_source::rs2): ALU_result = alu.operate(alu_op,rs1_val, rs2_val);break;
                        case(ALU_source::I_12bit_imm):ALU_result = alu.operate(alu_op,rs1_val, I_12bit_imm);break;
                        case(ALU_source::I_shamt_imm):ALU_result = alu.operate(alu_op,rs1_val, I_shamt_imm);break;
                        case(ALU_source::store_imm):ALU_result = alu.operate(alu_op,rs1_val, store_imm);break;
                    default: throw runtime_error("clear ALU src1 but unknown ALU src2");
                    }
                case rs2:
                    ALU_result = alu.operate(alu_op,rs1_val, I_12bit_imm);break;
                default: throw runtime_error("Unknown ALU source 1");

            }

            //pass on data from ID/EX Register
            pipeline_registers.EX_MEM_register.ALU_result = ALU_result;
            pipeline_registers.EX_MEM_register.Memory_op = pipeline_registers.ID_EX_register.Memory_op;
            pipeline_registers.EX_MEM_register.Memory_data_type = pipeline_registers.ID_EX_register.Memory_data_type;
            pipeline_registers.EX_MEM_register.rd_addr = pipeline_registers.ID_EX_register.rd_addr;
            pipeline_registers.EX_MEM_register.rs1_val = pipeline_registers.ID_EX_register.rs1_val;
            pipeline_registers.EX_MEM_register.rs2_val = pipeline_registers.ID_EX_register.rs2_val;
            pipeline_registers.EX_MEM_register.Store_imm = pipeline_registers.ID_EX_register.Store_imm;
            pipeline_registers.EX_MEM_register.Store_op = pipeline_registers.ID_EX_register.Store_op;
            pipeline_registers.EX_MEM_register.valid = 1;
            pipeline_registers.EX_MEM_register.ALU_result = ALU_result;
        }
        else {
            pipeline_registers.EX_MEM_register.valid = 0;
        }
    }
    void Memory(L2Cache &l2cache, RAM& ram) {
        if (pipeline_registers.EX_MEM_register.valid == 1) {
            Memory_op memory_op = pipeline_registers.EX_MEM_register.Memory_op;
            Memory_data_type memory_data_type = pipeline_registers.EX_MEM_register.Memory_data_type;
            uint32_t ALU_result = pipeline_registers.EX_MEM_register.ALU_result;
            l1_cache.Load(memory_op, memory_data_type,ALU_result,l2cache, ram); //Remember to connect this to I/O

            //pass on data from EX/MEM Register
            pipeline_registers.MEM_WB_register.ALU_result = pipeline_registers.EX_MEM_register.ALU_result;
            pipeline_registers.MEM_WB_register.rd_addr = pipeline_registers.EX_MEM_register.rd_addr;
            pipeline_registers.MEM_WB_register.rs1_val = pipeline_registers.EX_MEM_register.Store_imm;
            pipeline_registers.MEM_WB_register.Store_op = pipeline_registers.EX_MEM_register.Store_op;
            pipeline_registers.MEM_WB_register.valid = 1;
        }
        else {
            pipeline_registers.MEM_WB_register.valid = 0;
        }
    }
    void WriteBack(L2Cache& l2cache, RAM& ram) {
        if (pipeline_registers.MEM_WB_register.valid == 1) {
            uint32_t ALU_result = pipeline_registers.MEM_WB_register.ALU_result;
            Store_op Store_op = pipeline_registers.MEM_WB_register.Store_op;
            uint32_t Store_addr = ALU_result;
            uint32_t rd_addr = pipeline_registers.MEM_WB_register.rd_addr;
            uint32_t rs2_val = pipeline_registers.MEM_WB_register.rs2_val;

            registerFile.write(rd_addr,ALU_result);
            l1_cache.Store(Store_op,Store_addr,rs2_val, l2cache, ram);
        }
    }

    void Step(ProgramCounter program_counter, L2Cache &l2cache, RAM& ram) {
        Fetch(program_counter,l2cache,ram);
        Decode(pipeline_registers.IF_ID_register.machine_code);
        Execution();
        Memory(l2cache,ram);
        WriteBack(l2cache,ram);
    }

};

#endif //RISC_V_CPU_SIMULATOR_CPUCORE_H
