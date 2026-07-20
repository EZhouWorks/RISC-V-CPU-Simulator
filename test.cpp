//this is a test environment for each cpu core
#include "src/RegisterFile.h"
#include<iostream>
#include<string>
#include<bitset>
#include<array>
#include "src/CPUcore.h"
#include <vector>

using namespace std;
void dumpSelectedReg(CPUcore core, int reg1_index, int reg2_index, int reg3_index) {
    cout<<"Reg "<<reg1_index<<" :"<<core.registerFile.read(reg1_index)<<endl;
    cout<<"Reg "<<reg2_index<<" :"<<core.registerFile.read(reg2_index)<<endl;
    cout<<"Reg "<<reg3_index<<" :"<<core.registerFile.read(reg3_index)<<endl;
}
void varifyCommand(CPUcore core, vector<uint32_t> machine_code) {
    core.registerFile.write(1,3); //rs1 val
    for (uint32_t m_code : machine_code) {
        cout<<"machine code: "<<hex<<m_code<<endl;

        core.Step(m_code);
        cout<<"imm value: "<<core.decoder.I_shamt_imm<<endl;
        cout<<"r1 value: "<<core.registerFile.read(1)<<endl;
        cout<<"rd value: "<<core.registerFile.read(3)<<endl<<endl;
    }
}
void varifyL1Cache(CPUcore core) {
    L2Cache l2cache = L2Cache();
    RAM ram = RAM();
    ram.memory[0] = 0b10110011;
    ram.memory[1] = 0b11110001;
    ram.memory[2] = 0b00100000;
    ram.memory[3] = 0b00000000;
    l2cache.loadCacheBlockFromRAM(0,ram);
    cout<<bitset<32>(core.l1_cache.readFullData(0,l2cache,ram))<<endl;

    cout<<bitset<32>(core.l1_cache.readFullData(0,l2cache,ram))<<endl;


}

int main() {
    uint32_t machine_code = 0b00000000001000001111000110110011;
    CPUcore core0 = CPUcore(0);
    core0.registerFile.write(1,3); //rs1 val
    // core0.registerFile.write(2,4); //rs2 val
    // core0.Step(machine_code);
    // //dumpSelectedReg(core0,3,1,2);
    // core0.registerFile.dumpRawValue();
    vector<uint32_t> ITypeCommandSet = {
        //0b00000000 01010000 10000001 10010011,
        0b00000000010100001010000110010011,
        0b00000000010100001011000110010011,
        0b00000000010100001100000110010011,
        0b00000000010100001110000110010011,
        0b00000000010100001111000110010011,
        0b00000000001000001001000110010011,
        0b00000000001000001101000110010011,
        0b01000000001000001101000110010011
    };

    varifyL1Cache(core0);

}