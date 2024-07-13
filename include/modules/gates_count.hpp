#ifndef GATES_COUNT_HPP
#define GATES_COUNT_HPP

#include <iostream>

namespace gates_count {
// Just an XOR Gate
const int COMPARE_BIT = 1;

const int STORE_BIT = 4;
const int READ_BIT = 2;

// A multiplexer with n bits input has n not gates, 2^n and gates and 1 not gate
int gates_count_multiplexer(int outputs) {
    // number of outputs has to be power of 2
    if (outputs & (outputs - 1)) {
        return -1;
    }

    int not_gates = 0;
    while (outputs != 1) {
        not_gates++;
        outputs >>= 1;
    }

    int and_gates = outputs;
    int or_gates = 1;

    return not_gates + and_gates + or_gates;
}

const int CONTROLL_LOGIC = 100;
} // namespace gates_count_constants

#endif
