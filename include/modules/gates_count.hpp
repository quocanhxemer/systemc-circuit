#ifndef GATES_COUNT_HPP
#define GATES_COUNT_HPP

#include <iostream>

namespace gates_count {
// Just an XOR Gate
const int COMPARE_BIT = 1;

const int STORE_BIT = 4;
const int READ_BIT = 2;

// for addition and bit shift operations for 32 bits number
const int ALU = 200;

// A multiplexer with n bits input has n not gates, 2^n and gates and 1 not gate
int gates_count_decoder(int outputs) {
    // number of outputs has to be power of 2
    if (outputs & (outputs - 1)) {
        return -1;
    }

    // To decode and assign a k-bit cache index, we need (according to the truth
    // table) k not-gates for every bits, 2^k and-gates and one or gate
    // Example: 2-bit input and 4 outputs:
    // f = AB + (notA)B + A(notB) + (notA)(notB)

    // not_gates = log2 (outputs)
    int temp = outputs;
    int not_gates = 0;
    while (temp != 1) {
        not_gates++;
        temp >>= 1;
    }

    int and_gates = outputs;
    int or_gates = 1;

    return not_gates + and_gates + or_gates;
}

const int CONTROLL_LOGIC = 100;
} // namespace gates_count

#endif
