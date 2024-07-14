#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "cache.hpp"
#include "memory.hpp"

#include <systemc>

using namespace sc_core;

SC_MODULE(CONTROLLER) {

    sc_in<bool> clk;
    sc_in<uint32_t> address;
    sc_in<uint32_t> data_input;
    sc_in<int> we;

    sc_in<bool> trigger;

    sc_out<uint32_t> data_output;
    sc_out<bool> done;

    int gates_count;

    unsigned cacheLineSize;
    uint32_t data_output_temp;

    CACHE l1;
    CACHE l2;

    MEMORY memory;

    bool hit;

    bool running;

    sc_signal<uint32_t> aligned_address;
    sc_signal<int> offset_from;
    sc_signal<int> offset_to;
    sc_signal<uint8_t *> aligned_data_input;

    sc_signal<bool> l1_trigger;
    sc_signal<uint8_t *> l1_output;
    sc_signal<bool> l1_done;
    sc_signal<CACHE_STATUS> l1_status;

    sc_signal<bool> l2_trigger;
    sc_signal<uint8_t *> l2_output;
    sc_signal<bool> l2_done;
    sc_signal<CACHE_STATUS> l2_status;

    sc_signal<bool> memory_trigger;
    sc_signal<uint8_t *> memory_output;
    sc_signal<bool> memory_done;

    SC_CTOR(CONTROLLER);

    CONTROLLER(sc_module_name name, sc_clock & clk, unsigned l1CacheLines,
               unsigned l2CacheLines, unsigned cacheLineSize,
               unsigned l1CacheLatency, unsigned l2CacheLatency,
               unsigned memoryLatency, unsigned memorySize)
        : sc_module(name), cacheLineSize(cacheLineSize),
          l1("l1", clk, l1CacheLines, cacheLineSize, l1CacheLatency),
          l2("l2", clk, l2CacheLines, cacheLineSize, l2CacheLatency),
          memory("memory", clk, cacheLineSize, memoryLatency, memorySize) {

        this->clk.bind(clk);

        l1.address.bind(aligned_address);
        l1.offset_from.bind(offset_from);
        l1.offset_to.bind(offset_to);
        l1.data_input.bind(aligned_data_input);
        l1.we.bind(we);
        l1.trigger.bind(l1_trigger);
        l1.data_output.bind(l1_output);
        l1.done.bind(l1_done);
        l1.status.bind(l1_status);

        l2.address.bind(aligned_address);
        l2.offset_from.bind(offset_from);
        l2.offset_to.bind(offset_to);
        l2.data_input.bind(aligned_data_input);
        l2.we.bind(we);
        l2.trigger.bind(l2_trigger);
        l2.data_output.bind(l2_output);
        l2.done.bind(l2_done);
        l2.status.bind(l2_status);

        memory.address.bind(aligned_address);
        memory.offset_from.bind(offset_from);
        memory.offset_to.bind(offset_to);
        memory.data_input.bind(aligned_data_input);
        memory.we.bind(we);
        memory.trigger.bind(memory_trigger);
        memory.data_output.bind(memory_output);
        memory.done.bind(memory_done);

        gates_count = l1.gates_count + l2.gates_count;

        SC_CTHREAD(run, this->clk.pos());

        SC_THREAD(detect_trigger);
        sensitive << trigger;
    }

    void detect_trigger() {
        while (true) {
            wait();
            hit = true;
            running = true;
            data_output_temp = 0;
        }
    }

    void run() {
        while (true) {
            wait();
            if (running) {
                process();
                running = false;
                done->write(true);
                // sc_core::wait(1);
                // done->write(false);
            }
        }
    }

    void process() {
        uint32_t first_byte = address->read();
        uint32_t last_byte = first_byte + 3;

        // In case address isn't aligned with the cache line
        // or the cache line is smaller than 4 byte (whyyy)
        int cache_lines_count =
            cache_line_of(last_byte) - cache_line_of(first_byte) + 1;

        int aligned_offset = first_byte - get_aligned_address(first_byte);

        // convert data input to a 4 - element array
        uint8_t *data8 = data_32_to_8(data_input->read());

        // Loop through every cache line that needs to access
        if (cache_lines_count == 1) {
            access_line(get_aligned_address(first_byte), aligned_offset,
                        aligned_offset + 4, data8);
        } else {
            access_line(get_aligned_address(first_byte), aligned_offset,
                        cacheLineSize, data8);
            for (int i = 1; i < cache_lines_count - 1; i++) {
                access_line(get_aligned_address(first_byte) + i * cacheLineSize,
                            0, cacheLineSize,
                            data8 - aligned_offset + i * cacheLineSize);
            }
            // TODO: super ugly code
            int rest_bits = cacheLineSize - aligned_offset;
            access_line(get_aligned_address(last_byte), 0, 4 - rest_bits,
                        data8 + rest_bits);
        }

        delete[] data8;
    }

    void access_line(uint32_t address, int from, int to, uint8_t *data) {
        aligned_address.write(address);
        offset_from.write(from);
        offset_to.write(to);
        aligned_data_input.write(data);

        access(to - from);
    }

    void access(int bytes_accessed) {
        if (we->read()) {
            write();
        } else {
            uint8_t *d = read();
            for (int i = 0; i < bytes_accessed; i++) {
                data_output_temp <<= 8;
                data_output_temp += d[i];
            }
            data_output.write(data_output_temp);
            delete[] d;
        }
    }

    void write() {
        // Write through, don't care if cache hit or miss
        l1_run();
        l2_run();
        memory_run();
    }

    uint8_t *read() {
        l1_run();
        if (l1_status.read() == CACHE_HIT) {
            return l1_output.read();
        }
        l2_run();
        if (l2_status.read() == CACHE_HIT) {
            aligned_data_input.write(l2_output.read());
            l1.write_back();
            return l2_output.read();
        }
        hit = false;
        memory_run();
        aligned_data_input.write(memory_output.read());
        l2.write_back();
        l1.write_back();
        return memory_output.read();
    }

    void l1_run() {
        l1_trigger.write(!l1_trigger.read());
        do {
            wait();
        } while (!l1_done.read());
        // l1_done.write(false);
    }

    void l2_run() {
        l2_trigger.write(!l2_trigger.read());
        do {
            wait();
        } while (!l2_done.read());
        // l2_done.write(false);
    }

    void memory_run() {
        memory_trigger.write(!memory_trigger.read());
        do {
            wait();
        } while (!memory_done.read());
        // memory_done.write(false);
    }

    uint32_t get_aligned_address(uint32_t addr) {
        return addr & ~(cacheLineSize - 1);
    }

    uint32_t cache_line_of(uint32_t addr) {
        return get_aligned_address(addr) / cacheLineSize;
    }

    uint8_t *data_32_to_8(uint32_t data) {
        uint8_t *res = new uint8_t[4];
        res[0] = (data >> 24) & 0xff;
        res[1] = (data >> 16) & 0xff;
        res[2] = (data >> 8) & 0xff;
        res[3] = data & 0xff;
        return res;
    }
};

#endif
