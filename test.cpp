#include "Register.h"
#include<iostream>
#include<string>
#include<bitset>
using namespace std;
int main() {
    Register r = Register();
    r.write(0,20);
    r.dumpRawValue();
    return 0;
}