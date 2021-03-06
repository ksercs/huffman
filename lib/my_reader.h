#ifndef MY_READER_H
#define MY_READER_H

#include <fstream>
#include "constants.h"

struct My_reader {
    explicit My_reader(std::istream &);
    explicit My_reader(std::istream &, bool);

    uint8_t read_byte();
    decode_map read_table();
    void start_reading();

    bool is_end();
    bool is_penultimate();
    bool is_empty();

    void to_begin();

private:
    std::istream &is;
    std::vector <uint8_t> buf;
    size_t pos = 0;
    uint8_t unread_buf_size;
};


#endif //MY_READER_H
