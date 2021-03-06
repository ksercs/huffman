#include <iostream>

#include "my_writer.h"

My_writer::My_writer(std::ostream & _os) : os(_os) {}

void My_writer::write_byte(uint8_t byt) {
    os << byt;
}

void My_writer::write_table(huffman_table & codes) {
    for (auto it : codes) {
        os << it.first << " " << it.second << " ";
    }
}

void My_writer::write_ost(uint8_t ost) {
    os << ost;
}
