#ifndef MY_READER_H
#define MY_READER_H

#include <fstream>
#include "constants.h"

struct My_reader {

    explicit My_reader(std::istream &);
    explicit My_reader(std::istream &, bool);

    void read_next_section();
    uint8_t read_byte();
    decode_map read_table();

    bool is_end();
    bool get_last_symbol();
    bool is_zeroes_cnt();

    uint8_t get_zeroes_cnt() const;
    size_t get_file_size() const;

    void to_begin();

private:
    std::istream &is;
    VEC <uint8_t> buf;
    size_t file_size = 0;
    size_t pos = 0;
    size_t section_ind = 0;
    uint64_t read_size = 0; // сколько байтов прочитали
};


#endif //MY_READER_H
