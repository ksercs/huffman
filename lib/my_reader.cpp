#include <iostream>

#include "my_reader.h"

My_reader::My_reader(std::ifstream & _is) : is(_is), pos(0), unread_buf_size(BUF_SIZE) {
    buf.resize(BUF_SIZE);
}

My_reader::My_reader(std::ifstream & _is, bool fl) : is(_is) {
    buf.resize(BUF_SIZE);
}

void My_reader::start_reading() {
    size_t i = 0;
    for (; i < BUF_SIZE && !is.eof(); ++i) {
        buf[i] = (uint8_t)is.get();
    }
    buf.resize(i - 1);
    unread_buf_size = i - 1;
}

bool My_reader::is_penultimate() {
    return  unread_buf_size == 1;
}

decode_map My_reader::read_table() {
    decode_map mp;
    for (size_t i = 0; i < ALPHA; ++i) {
        uint64_t a, b;
        is >> a >> b;
//        std::cout << i << " : " << v8 << " " << v16 << "\n";
        if (b != 0) {
            mp[std::make_pair(a, b)] = (uint8_t)i;
        }
    }
    is.get();

    start_reading();

    return mp;
}

bool My_reader::is_empty() {
    is.seekg (0, is.end);
    size_t file_size = is.tellg();
    is.seekg (0, is.beg);
    return file_size == 0;
}

void My_reader::to_begin() {
    is.clear();
    is.seekg(std::ios::beg);
    pos = 0;
    unread_buf_size = buf.size();
    start_reading();
}

void smp_logging(uint8_t c) {
    std::string s = "";
    for (int i = 7; i >= 0; --i) {
        s += (((c & ((uint8_t)1 << i)) > 0) + '0');
    }
    std::cout << s << "\n";
}

uint8_t My_reader::read_byte() {
    uint8_t res = buf[pos];
//    smp_logging(res);
//    std::cout << pos << " : " << res << "\n";
    if (!is.eof()) {
        buf[pos] = (uint8_t)is.get();
    } else {
        --unread_buf_size;
    }
    pos = (pos + 1) % 3;
    return res;
}

bool My_reader::is_end() {
    return is.eof() && !unread_buf_size;
}
