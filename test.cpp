#include "src/RegisterFile.h"
#include<iostream>
#include<string>
#include<bitset>
#include <array>
using namespace std;
int main() {
    // Register r = Register();
    // r.write(0,20);
    // r.dumpRawValue();
    // return 0;
    uint32_t a = 0b0101000;
    int b = a;
    uint32_t bit0 = a & 1;
    cout<<bit0<<endl;
}