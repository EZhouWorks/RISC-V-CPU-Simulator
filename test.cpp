//this is a test environment for each cpu core
#include "src/RegisterFile.h"
#include<iostream>
#include<string>
#include<bitset>
#include "src/CPUcore.h"
using namespace std;
void dumpSelectedReg(CPUcore core, int reg1_index, int reg2_index, int reg3_index) {
    cout<<"Reg "<<reg1_index<<" :"<<core.registerFile.read(reg1_index)<<endl;
    cout<<"Reg "<<reg2_index<<" :"<<core.registerFile.read(reg2_index)<<endl;
    cout<<"Reg "<<reg3_index<<" :"<<core.registerFile.read(reg3_index)<<endl;
}

int main() {
    //| R | `add x3, x1, x2` | `0000000 00010 00001 000 00011 0110011` |
    uint32_t machine_code = 0b00000000001000001111000110110011;
    CPUcore core0 = CPUcore(0);
    core0.registerFile.write(1,3); //rs1 val
    core0.registerFile.write(2,4); //rs2 val
    core0.Step(machine_code);
    //dumpSelectedReg(core0,3,1,2);
    core0.registerFile.dumpRawValue();

}