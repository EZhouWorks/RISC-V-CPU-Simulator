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
#include "ForwardingUnit.h"
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
    PipelineRegisters pipeline_registers_write = PipelineRegisters(); //this pipeline register set is write only
    PipelineRegisters pipeline_registers_read = PipelineRegisters();  //this pipeline register set is read only
    ProgramCounter program_counter = ProgramCounter(0b0,27);
    ForwardingUnit forwarding_unit = ForwardingUnit();

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

    void Fetch(L2Cache& l2cache,RAM& ram) {
        if (program_counter.CheckValid() == 1) {
            pipeline_registers_write.IF_ID_register.machine_code = CPULoadWord(program_counter.PC_value,l2cache,ram);
            pipeline_registers_write.IF_ID_register.valid = 1;
            program_counter.StepForward(4);
        }
        else {
            pipeline_registers_write.IF_ID_register.valid = 0;
        }

        cout<<"FETCH valid "<<pipeline_registers_write.IF_ID_register.valid<<endl;
    }
    void Decode() {
        cout<<"decode valid "<<pipeline_registers_read.IF_ID_register.valid<<endl;
        if (pipeline_registers_read.IF_ID_register.valid == 1) {
            controller.SetControlSignal(decoder.Decode(pipeline_registers_read.IF_ID_register.machine_code)); //decode and generate control singal
            //store data and control signal into pipeline register
            pipeline_registers_write.ID_EX_register.ALU_operation = controller.ALU_operation;
            pipeline_registers_write.ID_EX_register.ALU_source1 = controller.ALU_source1;
            pipeline_registers_write.ID_EX_register.ALU_source2 = controller.ALU_source2;
            pipeline_registers_write.ID_EX_register.Memory_op = controller.Memory_op;
            pipeline_registers_write.ID_EX_register.Memory_data_type = controller.Memory_data_type;

            pipeline_registers_write.ID_EX_register.rs1_addr = decoder.rs1;
            pipeline_registers_write.ID_EX_register.rs2_addr = decoder.rs2;
            pipeline_registers_write.ID_EX_register.rd_addr = decoder.rd;
            //WB Bypass
            if (pipeline_registers_read.MEM_WB_register.valid == 1 and forwarding_unit.ForwardingCompare(pipeline_registers_read.MEM_WB_register.rd_addr,decoder.rs1) == 1) {
                pipeline_registers_write.ID_EX_register.rs1_val = pipeline_registers_read.MEM_WB_register.ALU_result;
            }
            else {
                pipeline_registers_write.ID_EX_register.rs1_val = registerFile.read(decoder.rs1);
            }
            if (pipeline_registers_read.MEM_WB_register.valid == 1 and forwarding_unit.ForwardingCompare(pipeline_registers_read.MEM_WB_register.rd_addr,decoder.rs2) == 1) {
                pipeline_registers_write.ID_EX_register.rs2_val = pipeline_registers_read.MEM_WB_register.ALU_result;
            }
            else {
                pipeline_registers_write.ID_EX_register.rs2_val = registerFile.read(decoder.rs2);
            }

            pipeline_registers_write.ID_EX_register.I_12bit_imm = decoder.I_12bit_imm;
            pipeline_registers_write.ID_EX_register.I_shamt_imm = decoder.I_shamt_imm;
            pipeline_registers_write.ID_EX_register.Memory_op = controller.Memory_op;
            pipeline_registers_write.ID_EX_register.Store_imm = decoder.Store_imm;
            pipeline_registers_write.ID_EX_register.Store_op = controller.Store_op;
            pipeline_registers_write.ID_EX_register.valid = 1;
        }
        else {
            pipeline_registers_write.ID_EX_register.valid = 0;
        }
    }
    void Execution() {
        cout<<"EX VALID "<<pipeline_registers_read.ID_EX_register.valid<<endl;
        if (pipeline_registers_read.ID_EX_register.valid == 1) {
            uint32_t ALU_result = 0b0;
            ALU_op alu_op = pipeline_registers_read.ID_EX_register.ALU_operation;
            ALU_source alu_source1 = pipeline_registers_read.ID_EX_register.ALU_source1;
            ALU_source alu_source2 = pipeline_registers_read.ID_EX_register.ALU_source2;
            uint32_t rs1_val = pipeline_registers_read.ID_EX_register.rs1_val;
            uint32_t rs2_val = pipeline_registers_read.ID_EX_register.rs2_val;
            uint32_t rs1_addr = pipeline_registers_read.ID_EX_register.rs1_addr;
            uint32_t rs2_addr = pipeline_registers_read.ID_EX_register.rs2_addr;
            uint32_t rs1_input_val = rs1_val;
            uint32_t rs2_input_val = rs2_val;
            uint32_t I_12bit_imm = pipeline_registers_read.ID_EX_register.I_12bit_imm;
            uint32_t I_shamt_imm = pipeline_registers_read.ID_EX_register.I_shamt_imm;
            uint32_t store_imm = pipeline_registers_read.ID_EX_register.Store_imm;

            //forwarding check
            if (pipeline_registers_write.EX_MEM_register.valid == 1 and forwarding_unit.ForwardingCompare(rs1_addr,pipeline_registers_read.EX_MEM_register.rd_addr) == 1) {
                rs1_input_val = pipeline_registers_read.EX_MEM_register.ALU_result; //fetch ALU result from the read register of the next stage
            }
            if (pipeline_registers_write.EX_MEM_register.valid == 1 and forwarding_unit.ForwardingCompare(rs2_addr,pipeline_registers_read.EX_MEM_register.rd_addr) == 1) {
                rs2_input_val = pipeline_registers_read.EX_MEM_register.ALU_result;
            }
            if (pipeline_registers_write.MEM_WB_register.valid == 1 and forwarding_unit.ForwardingCompare(rs1_addr,pipeline_registers_read.MEM_WB_register.rd_addr) == 1) {
                rs1_input_val = pipeline_registers_read.MEM_WB_register.ALU_result;
            }
            if (pipeline_registers_write.MEM_WB_register.valid == 1 and forwarding_unit.ForwardingCompare(rs2_addr,pipeline_registers_read.MEM_WB_register.rd_addr) == 1) {
                rs2_input_val = pipeline_registers_read.MEM_WB_register.ALU_result;
            }
            cout<<"=============="<<endl;
            cout<<"rs1 "<<rs1_val<<endl;
            cout<<"rs2 "<<rs2_val<<endl;
            cout<<"rs1_input "<<rs1_input_val<<endl;
            cout<<"rs2_input "<<rs2_input_val<<endl;
            cout<<"=============="<<endl;

            //ALU execution
            switch (alu_source1) {
                case rs1:
                    switch (alu_source2) {
                        case(ALU_source::rs2): ALU_result = alu.operate(alu_op,rs1_input_val, rs2_input_val);break;
                        case(ALU_source::I_12bit_imm):ALU_result = alu.operate(alu_op,rs1_input_val, I_12bit_imm);break;
                        case(ALU_source::I_shamt_imm):ALU_result = alu.operate(alu_op,rs1_input_val, I_shamt_imm);break;
                        case(ALU_source::store_imm):ALU_result = alu.operate(alu_op,rs1_input_val, store_imm);break;
                    default: throw runtime_error("clear ALU src1 but unknown ALU src2");
                    };break;
                case rs2:
                    ALU_result = alu.operate(alu_op,rs1_input_val, I_12bit_imm);break;
                default: throw runtime_error("Unknown ALU source 1");

            }

            //pass on data from ID/EX Register
            pipeline_registers_write.EX_MEM_register.ALU_result = ALU_result;
            pipeline_registers_write.EX_MEM_register.Memory_op = pipeline_registers_read.ID_EX_register.Memory_op;
            pipeline_registers_write.EX_MEM_register.Memory_data_type = pipeline_registers_read.ID_EX_register.Memory_data_type;
            pipeline_registers_write.EX_MEM_register.rd_addr = pipeline_registers_read.ID_EX_register.rd_addr;
            pipeline_registers_write.EX_MEM_register.rs1_val = pipeline_registers_read.ID_EX_register.rs1_val;
            pipeline_registers_write.EX_MEM_register.rs2_val = pipeline_registers_read.ID_EX_register.rs2_val;
            pipeline_registers_write.EX_MEM_register.Store_imm = pipeline_registers_read.ID_EX_register.Store_imm;
            pipeline_registers_write.EX_MEM_register.Store_op = pipeline_registers_read.ID_EX_register.Store_op;
            pipeline_registers_write.EX_MEM_register.ALU_result = ALU_result;
            pipeline_registers_write.EX_MEM_register.valid = 1;
        }
        else {
            pipeline_registers_write.EX_MEM_register.valid = 0;
        }
    }
    void Memory(L2Cache &l2cache, RAM& ram) {
        cout<<"MEM valid "<<pipeline_registers_read.EX_MEM_register.valid<<endl;
        if (pipeline_registers_read.EX_MEM_register.valid == 1) {
            Memory_op memory_op = pipeline_registers_read.EX_MEM_register.Memory_op;
            Memory_data_type memory_data_type = pipeline_registers_read.EX_MEM_register.Memory_data_type;
            uint32_t ALU_result = pipeline_registers_read.EX_MEM_register.ALU_result;
            l1_cache.Load(memory_op, memory_data_type,ALU_result,l2cache, ram); //Remember to connect this to I/O

            //pass on data from EX/MEM Register
            pipeline_registers_write.MEM_WB_register.ALU_result = pipeline_registers_read.EX_MEM_register.ALU_result;
            pipeline_registers_write.MEM_WB_register.rd_addr = pipeline_registers_read.EX_MEM_register.rd_addr;
            pipeline_registers_write.MEM_WB_register.rs1_val = pipeline_registers_read.EX_MEM_register.Store_imm;
            pipeline_registers_write.MEM_WB_register.Store_op = pipeline_registers_read.EX_MEM_register.Store_op;
            pipeline_registers_write.MEM_WB_register.valid = 1;
        }
        else {
            pipeline_registers_write.MEM_WB_register.valid = 0;
        }
    }
    void WriteBack(L2Cache& l2cache, RAM& ram) {
        cout<<"WB Valid "<<pipeline_registers_read.MEM_WB_register.valid<<endl;
        if (pipeline_registers_read.MEM_WB_register.valid == 1) {
            uint32_t ALU_result = pipeline_registers_read.MEM_WB_register.ALU_result;
            Store_op Store_op = pipeline_registers_read.MEM_WB_register.Store_op;
            uint32_t Store_addr = ALU_result;
            uint32_t rd_addr = pipeline_registers_read.MEM_WB_register.rd_addr;

            cout<<"WB "<<rd_addr<<endl;
            cout<<"ALU RESULT WB "<<ALU_result<<endl;
            uint32_t rs2_val = pipeline_registers_read.MEM_WB_register.rs2_val;

            registerFile.write(rd_addr,ALU_result);
            l1_cache.Store(Store_op,Store_addr,rs2_val, l2cache, ram);
        }
    }

    void Step(L2Cache &l2cache, RAM& ram) {
        Fetch(l2cache,ram);
        Decode();
        Execution();
        Memory(l2cache,ram);
        WriteBack(l2cache,ram);
        pipeline_registers_read = pipeline_registers_write; //flip value every cycle
    }
};

#endif //RISC_V_CPU_SIMULATOR_CPUCORE_H
