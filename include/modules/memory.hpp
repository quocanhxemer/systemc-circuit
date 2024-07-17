#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <systemc>
#include <unordered_map>

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

    std::unordered_map<uint32_t, uint8_t *> data_map;

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

        SC_CTHREAD(run, this->clk.pos());

        SC_THREAD(detect_trigger);
        sensitive << trigger;
    }

    ~MEMORY() {
        for (auto it = data_map.begin(); it != data_map.end(); it++) {
            delete[] it->second;
        }
    }

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

        // initialize the address block if it has never been accessed
        if (data_map.find(address->read()) == data_map.end()) {
            data_map[address->read()] = new uint8_t[block_size];
            for (int i = 0; i < block_size; i++) {
                data_map[address->read()][i] = 0;
            }
        }

        if (we->read()) {
            write();
        } else {
            read();
        }
    }

    void write() {
        for (int i = offset_from->read(); i < offset_to->read(); i++) {
            data_map[address->read()][i] =
                data_input->read()[i - offset_from->read()];
        }
    }

    void read() {
        uint8_t *output = new u_int8_t[offset_to->read() - offset_from->read()];
        for (int i = offset_from->read(); i < offset_to->read(); i++) {
            output[i - offset_from->read()] = data_map[address->read()][i];
        }
        data_output->write(output);
    }
};

#endif
