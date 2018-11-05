#include <iostream>

#include "my_reader.h"

My_reader::My_reader(std::istream & _is) : is(_is), pos(0), section_ind(0) {
    is.seekg (0, is.end);
    file_size = is.tellg();
    std::cout << "IN SIZE : " << file_size << "\n";
    is.seekg (0, is.beg);
    read_next_section();
}

My_reader::My_reader(std::istream & _is, bool fl) : is(_is) {
    is.seekg (0, is.end);
    file_size = is.tellg();
    if (file_size != 0) {
        file_size -= 4096;
    }
    std::cout << "OUT SIZE : " << (file_size == 0 ? 0 : file_size - 1) << "\n";
    is.seekg (0, is.beg);
}

bool My_reader::get_last_symbol() {
    return read_size == file_size && pos == buf.size();
}

bool My_reader::is_zeroes_cnt() {
    return read_size == file_size && pos == buf.size() - 1;
}

uint8_t My_reader::get_zeroes_cnt() const {
    return buf.back();
}

decode_map My_reader::read_table() {
    decode_map mp;
    VEC <unsigned char> v(2 * ALPHA * 8);
    is.read(reinterpret_cast<char *>(v.data()), 2 * ALPHA * 8);
    //std::cout << "READ : " << is.gcount() << "\n";
    for (size_t i = 0; i < 2 * ALPHA * 8; i += 16) {
        uint64_t a = 0, b = 0;
        for (int j = 7; j >= 0; --j) {
            uint64_t c = (uint64_t)v[i + 8 - j - 1];
            //std::cout << i + 8 - j - 1 << " : " << c << "\n";
            a |= (c << (((8 - j - 1) * 8)));
        }
        //std::cout << "\n--------\n";
        for (int j = 7; j >= 0; --j) {
            uint64_t c = (uint64_t)v[i + 16 - j - 1];
            //std::cout << i + 16 - j - 1 << " : " << c << "\n";
            b |= (c << (((8 - j - 1) * 8)));
            //std::cout << b << "\n";
        }
        if (b != 0) {
            mp[{a, b}] = i / 16;
        }
        //std::cout << "\n<<<<<<<<<<-------->>>>>>>>>\n";
    }
    //std::cout << "HERE : " << mp[{0, 1}] << "\n";
    return mp;
}

void My_reader::read_next_section() {
    buf.resize(BLOCK_SIZE);
    is.read(reinterpret_cast<char *>(buf.data()), BLOCK_SIZE);
    //std::cout << "READ SECTION " << is.gcount() << "\n";
    buf.resize(is.gcount());
    pos = 0;
    read_size += is.gcount();
    ++section_ind;
}

size_t My_reader::get_file_size() const {
    return file_size;
}

void My_reader::to_begin() {
    is.clear();
    is.seekg(std::ios::beg);
    pos = section_ind = read_size = 0;
    read_next_section();
}

uint8_t My_reader::read_byte() {
    uint8_t res = buf[pos++];
    if (pos == buf.size() && buf.size() == BLOCK_SIZE) {
        read_next_section();
    }
    return res;
}

bool My_reader::is_end() {
    //std::cout << "HERE : " << (section_ind - 1) * BLOCK_SIZE + pos << " " << file_size << "\n";
    return ((section_ind - 1) * BLOCK_SIZE + pos) == file_size;
}
