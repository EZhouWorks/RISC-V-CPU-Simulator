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

void varifyPipeline(CPUcore &core, L2Cache& l2cache, RAM &ram) {
    uint32_t commands[] = {

        // addi x1, x0, 5
        0b00000000010100000000000010010011,

        // addi x2, x0, 10
        0b00000000101000000000000100010011,

        // add x3, x1, x2
        0b00000000001000001000000110110011,
        //
        // sub x4, x2, x1
        0b01000000000100010000001000110011,
        //
        // and x5, x1, x2
        0b00000000001000001111001010110011,

        // or x6, x1, x2
        0b00000000001000001110001100110011,

        // xor x7, x1, x2
        0b00000000001000001100001110110011,

        // sw x3, 0(x0)
        0b00000000001100000010000000100011
    };
    ram.loadCommands(commands, 7,0);
    for (int i=0;i<20;i++) {
        cout<<endl;
        cout<<"Cycle "<<i<<endl;
        core.Step(l2cache,ram);
    }
    core.registerFile.dumpRawValue();

}


int main() {
    RAM ram = RAM();
    L2Cache l2cache = L2Cache(ram);
    CPUcore core0 = CPUcore(0);
    varifyPipeline(core0,l2cache,ram);
}