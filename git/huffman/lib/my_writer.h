#ifndef MY_WRITER_H
#define MY_WRITER_H

#include <algorithm>
#include <utility>
#include <fstream>

#include "constants.h"

struct My_writer {

    My_writer(std::ofstream &);

    void write_buf();
    void write_table(huffman_table &);
    void write_ost(uint8_t);

    void write_byte(uint8_t);
private:
    std::ofstream & os;
};

#endif //MY_WRITER_H
