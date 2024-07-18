#ifndef CACHE_HPP
#define CACHE_HPP

#include "gates_count.hpp"
#include <systemc>

using namespace sc_core;

enum CACHE_STATUS {
    CACHE_HIT,
    CACHE_MISS,
};

struct cache_line {
    uint8_t *data;
    uint32_t tag;
    bool valid;

    // Only counts READ hits/misses, since caches are write-through
    int hits_count;
    int misses_count;
};

// E.g. Cache line size = 4 bytes, write 0x02030405 to address 0x0002 (mapped to
// cache line 0, offset 2) then CACHE will run 2 write operations
// 1. data_input = [0x02, 0x03], address 0x0000, offset_from = 2, offset_to = 4
// 2. data_input = [0x04, 0x05], address 0x0004, offset_from = 0, offset_to = 2
SC_MODULE(CACHE) {

    sc_in<bool> clk;
    sc_in<uint32_t> address;
    sc_in<int> offset_from;
    sc_in<int> offset_to;
    sc_in<uint8_t *> data_input;
    sc_in<int> we;

    // cache only starts running when trigger signal changes
    sc_in<bool> trigger;

    sc_out<uint8_t *> data_output;
    sc_out<bool> done;
    sc_out<CACHE_STATUS> status;

    cache_line *caches;

    int gates_count;

    unsigned cache_lines;
    unsigned cache_line_size;
    unsigned latency;

    bool running = false;

    SC_CTOR(CACHE);

    CACHE(sc_module_name name, sc_clock & clk, unsigned cache_lines,
          unsigned cache_line_size, unsigned latency)
        : sc_module(name), cache_lines(cache_lines),
          cache_line_size(cache_line_size), latency(latency) {

        this->clk.bind(clk);

        caches = new cache_line[cache_lines];
        for (int i = 0; i < cache_lines; i++) {
            caches[i].data = new uint8_t[cache_line_size];
            for (int k = 0; k < cache_line_size; k++) {
                caches[i].data[k] = 0;
            }
            caches[i].valid = false;
            caches[i].hits_count = 0;
            caches[i].misses_count = 0;
        }

        gates_count = get_gates_count();

        SC_CTHREAD(run, this->clk.pos());

        SC_THREAD(detect_trigger);
        sensitive << trigger;
    }

    ~CACHE() {
        for (int i = 0; i < cache_lines; i++) {
            delete[] caches[i].data;
        }
        delete[] caches;
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
        unsigned line = cache_line_of(address->read());
        uint32_t tag = tag_of(address->read());

        if (we->read()) {
            // Don't care if hit or miss,
            // since the caches are write-through anyway
            write_to_cache_line(line);
        } else {
            if (caches[line].valid && tag == caches[line].tag) {
                status->write(CACHE_HIT);
                caches[line].hits_count++;
                read_from_cache_line(line);
            } else {
                caches[line].misses_count++;
                status->write(CACHE_MISS);
            }
        }
    }

    void write_to_cache_line(unsigned line) {
        for (int i = offset_from->read(); i < offset_to->read(); i++) {
            caches[line].data[i] = data_input->read()[i - offset_from->read()];
        }
        caches[line].valid = true;
    }

    void read_from_cache_line(unsigned line) {
        uint8_t *output = new u_int8_t[offset_to->read() - offset_from->read()];
        for (int i = offset_from->read(); i < offset_to->read(); i++) {
            output[i - offset_from->read()] = caches[line].data[i];
        }
        data_output->write(output);
    }

    // n has to be power of 2
    int log2(int n) {
        int res = 0;
        while (n != 1) {
            n >>= 1;
            res++;
        }
        return res;
    }

    int offset_bits_count() { return log2(cache_line_size); }

    int index_bits_count() { return log2(cache_lines); }

    int tag_bits_count() {
        return 32 - offset_bits_count() - index_bits_count();
    }

    int get_gates_count() {
        // Comparing tags for hits/misses logics
        int tags_compare = gates_count::COMPARE_BIT * tag_bits_count();
        int storage =
            gates_count::STORE_BIT * 8 * cache_lines * cache_line_size;
        int read = gates_count::READ_BIT * 8 * cache_lines * cache_line_size;
        int alu = gates_count::ALU;
        int decoder = gates_count::gates_count_decoder(cache_lines);
        int controller = gates_count::CONTROLL_LOGIC;

        return tags_compare + storage + read + alu + decoder + controller;
    }

    // Write back after read - cache miss. Has no latency
    void write_back() { write_to_cache_line(cache_line_of(address->read())); }

    unsigned cache_line_of(uint32_t adr) {
        return (adr / cache_line_size) % cache_lines;
    }

    uint32_t tag_of(uint32_t adr) {
        return adr / cache_line_size / cache_lines;
    }
};

#endif
