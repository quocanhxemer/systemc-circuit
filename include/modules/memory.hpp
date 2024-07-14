#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <systemc>

using namespace sc_core;

SC_MODULE(MEMORY) {

    sc_in<bool> clk;
    sc_in<uint32_t> address;
    sc_in<int> offset_from;
    sc_in<int> offset_to;
    sc_in<uint8_t *> data_input;
    sc_in<int> we;

    // memory only starts running when trigger signal changes
    sc_in<bool> trigger;

    sc_out<uint8_t *> data_output;
    sc_out<bool> done;

    uint8_t *data;

    unsigned block_size;
    unsigned latency;
    unsigned memory_size;

    bool running = false;

    SC_CTOR(MEMORY);

    MEMORY(sc_module_name name, sc_clock & clk, unsigned block_size,
           unsigned latency, unsigned memory_size)
        : sc_module(name), block_size(block_size), latency(latency),
          memory_size(memory_size) {

        this->clk.bind(clk);

        data = new uint8_t[memory_size];
        for (int i = 0; i < memory_size; i++) {
            data[i] = 0;
        }

        SC_CTHREAD(run, this->clk.pos());

        SC_THREAD(detect_trigger);
        sensitive << trigger;
    }

    ~MEMORY() { delete[] data; }

    void detect_trigger() {
        while (true) {
            wait();
            running = true;
        }
    }

    void run() {
        while (true) {
            wait();
            if (!running) {
                continue;
            }
            access();
            running = false;
            done->write(true);
            wait(1);
            done->write(false);
        }
    }

    void access() {
        wait(latency);

        if (we->read()) {
            write();
        } else {
            read();
        }
    }

    void write() {
        for (int i = offset_from->read(); i < offset_to->read(); i++) {
            data[address->read() + i] =
                data_input->read()[i - offset_from->read()];
        }
    }

    void read() {
        uint8_t *output = new u_int8_t[offset_to->read() - offset_from->read()];
        for (int i = offset_from->read(); i < offset_to->read(); i++) {
            output[i - offset_from->read()] = data[address->read() + i];
        }
        data_output->write(output);
    }
};

#endif
