#pragma once
#include <list>
#include <vector>
#include <iterator>
#include <thread>

#include <iostream>

struct TapeParams {
    int write_delay;
    int read_delay;
    int move_delay;
    int rewind_delay;
};

static TapeParams tp;

class TapeInterface {
    protected:
    std::vector<int32_t>* tape;
    std::vector<int32_t>::iterator tape_begin;
    std::vector<int32_t>::iterator head_position;
    public:
       virtual std::vector<int32_t>* GetTapePointer() = 0;
       virtual void SetTapePointer(uint size) = 0;
       virtual void move_back_once() = 0;
       virtual void move_back_n(int n) = 0;
       virtual void move_once() = 0;
       virtual void move_n(int n) = 0;
       virtual void write(int index, int32_t to_write) = 0;
       virtual int32_t& read() = 0;
       virtual int32_t& operator[](int index) = 0;
};

class Tape : TapeInterface {
    public:
        void SetTapePointer(uint size) {
            tape = new std::vector<int32_t>(size);
            head_position = (*tape).begin();
        }

        std::vector<int32_t>* GetTapePointer() {
            return tape;
        }

        void move_once() {
            std::this_thread::sleep_for(std::chrono::milliseconds(tp.move_delay));
            ++head_position;
        }
        void move_back_once() {
            std::this_thread::sleep_for(std::chrono::milliseconds(tp.rewind_delay));
            --head_position;
        }
        void move_back_n(int n) {
            for (int i = 0; i < n; ++i) {
                move_back_once();
            }
        }
        void write(int index, int32_t to_write) {
            int32_t& element = (*this)[index];
            element = to_write;
            std::this_thread::sleep_for(std::chrono::milliseconds(tp.write_delay));
        }
        int32_t& read() {
            std::this_thread::sleep_for(std::chrono::milliseconds(tp.read_delay));
            //return (*tape)[index];
            return *head_position;
        }
        void move_n(int n) {
            for (int i = 0; i < n; ++i) {
                move_once();
            }
        }
        int32_t& operator[](int index) {
            int head_index = std::distance((*tape).begin(), head_position);
            int steps = std::labs(head_index - index);
            if (index > (int)head_index)
            {
                move_n(steps);
            }
            else {
                move_back_n(steps);
            }
            return read();
        }
};