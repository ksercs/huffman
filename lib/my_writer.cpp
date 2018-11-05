#include <iostream>

#include "my_writer.h"

My_writer::My_writer(std::ostream & _os) : os(_os), pos(0) {
    buf.resize(BLOCK_SIZE);
}

void My_writer::write_buf() {
    os.write(reinterpret_cast<const char *>(buf.data()), pos);
    //std::cout << pos << " " << pos_in_byte << "\n";
    //file_size += pos + (pos_in_byte != 0);
    //std::cout << file_size << "\n";
    std::fill(buf.begin(), buf.end(), 0);
    pos = 0;
}

void My_writer::push(uint8_t byt) {
    buf[pos++] = byt;
    if (pos == buf.size()) {
        write_buf();
    }
}

void My_writer::write_table(huffman_table & codes) {
    std::vector <uint64_t> v;
    for (auto it : codes) {
        v.push_back(it.first);
        v.push_back(it.second);
    }
//    std::cout << "CODES SIZE : " << codes.size() << "\n";
//    for (size_t i = 0; i < v.size(); i += 2) {
//        std::cout << v[i] << " " << v[i + 1] << "\n";
//    }
//    std::cout << "\n";
    os.write(reinterpret_cast<const char *>(v.data()), 2 * ALPHA * 8);
}

void My_writer::write_ost(uint8_t ost) {
    //os.write(reinterpret_cast<const char *>(ost), 1);
    os.put(char(ost));
    //file_size += 1;
    //std::cout << file_size << "\n";
}
