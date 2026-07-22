//This file defines the 32 register
//the values are stored in 32bit binary and the value passed in will be automatically converted to binary
#ifndef RISC_V_CPU_SIMULATOR_REGISTER_H
#define RISC_V_CPU_SIMULATOR_REGISTER_H
#include <cstdint>
#include <array>
#include <iostream>
#include <bitset>
using namespace std;
class RegisterFile {
private:
    array<uint32_t,32> registers;
public:
    RegisterFile() {
        for (int i=0; i<32; i++) {
            registers[i] = 00000000000000000000000000000000;
        }
    };

    uint32_t read(uint32_t reg_index) {
        return registers[reg_index];
    }

    void write(uint8_t index, uint32_t value) {
        if (index != 0) {
            registers[index] = value;
        }
    }

    void reset() {
        for (int i=0; i<32; i++) {
            registers[i] = 0;
        }
    }

    void dump() {
        for (int i=0; i<32; i++) {
            cout<<registers[i]<<" ";
        }
    }

    //functions below are for debug purposes only
    void toHex(uint32_t reg_index) {
        cout<<"0x"<<hex<<registers[reg_index];
    }

    void dumpRawValue() {
        for (int i=0; i<32; i++) {
            // cout<<"reg "<<i<<": 0b"<<bitset<32>(registers[i])<<endl;
            cout<<"reg "<<i<<": "<<registers[i]<<endl;
        }
    }

};
#endif //RISC_V_CPU_SIMULATOR_REGISTER_H



