#include <cstdint>
//
// Created by zhoue on 2026/7/23.
//
class ForwardingUnit {
public:
    ForwardingUnit() {}
    int ForwardingCompare(uint32_t source_addr, uint32_t current_addr = 0 ) {
        if (source_addr == current_addr) {
            return 1;
        }
        else {
            return 0;
        }
    }
};
#ifndef RISCV_CPU_FORWARDINGUNIT_H
#define RISCV_CPU_FORWARDINGUNIT_H

#endif //RISCV_CPU_FORWARDINGUNIT_H
