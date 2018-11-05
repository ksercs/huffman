#ifndef MY_WRITER_H
#define MY_WRITER_H

#include <algorithm>
#include <utility>
#include <fstream>

#include "constants.h"

struct My_writer {

    My_writer(std::ostream &);

    void write_buf();
    void write_table(huffman_table &);
    void write_ost(uint8_t);

    void push(uint8_t);
private:
    std::ostream & os;
    VEC <uint8_t> buf;
    size_t pos = 0;
    size_t file_size = 0;
};

#endif //MY_WRITER_H
