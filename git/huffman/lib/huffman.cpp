#include <fstream>
#include <iostream>

#include "huffman.h"

void build_table(My_reader & in, std::vector <uint64_t> & num) {
    while (!in.is_end()) {
        uint8_t byt = in.read_byte();
        ++num[byt];
    }
}

void logging(uint8_t c, size_t len) {
    std::string s = "";
    for (int i = 7; i >= 0; --i) {
        s += (((c & ((uint8_t)1 << i)) > 0) + '0');
    }
    std::cout << "RES_BYTE " << s << " " << len << "\n";
}

bool check_endian() { // 0 = little, 1 = big
    uint32_t a = 0x12345678;
    uint8_t * b = (uint8_t *)(&a);
    return (*b != 0x78);
}

void write_encode(My_reader & in, My_writer & out, huffman_table & codes) {
    in.to_begin();
    out.write_table(codes);

    uint8_t ost = 8,
            res_byte = 0,
            pos_in_byte = 0;
    while (!in.is_end()) {
        uint8_t byt = in.read_byte();
        auto it = codes[byt];
        auto cur_code = it.first;
        auto cur_code_len = it.second;

        while (cur_code_len >= ost) {
            size_t rub = (64 - cur_code_len);
            cur_code <<= rub >>= rub;
            res_byte |= (cur_code >> (cur_code_len - ost));
            out.write_byte(res_byte);
            res_byte = 0;
            cur_code_len -= ost;
            ost = 8;
            pos_in_byte = 0;
        }
        if (cur_code_len < ost) {
            size_t rub = (64 - cur_code_len);
            cur_code <<= rub >>= rub;
            res_byte |= (cur_code << (ost - cur_code_len));
            pos_in_byte += cur_code_len;
            ost -= cur_code_len;
            cur_code_len = 0;
        }
    }
    if (pos_in_byte) {
        out.write_byte(res_byte);
    }
    out.write_ost(ost % 8);
}

void encode(std::ifstream & fin, std::ofstream & fout) {
    My_reader in(fin);
    My_writer out(fout);
    std::vector <uint64_t> num(ALPHA);

    if (in.is_empty()) {
        return;
    }

    in.start_reading();
    huffman_table codes(ALPHA);
    build_table(in, num);
    make_tree(num, codes);
    write_encode(in, out, codes);
}

void write_decode(My_reader & in, My_writer & out, decode_map & codes) {
    uint64_t res_byte = 0,
             res_len  = 0;
    int fl = 0;
    while (!in.is_end()) {
        uint8_t byt = in.read_byte();
        if (in.is_penultimate()) {
            fl = in.read_byte();
        }
        for (int i = 7; i >= fl; --i) {
            uint8_t bit = (((1 << i) & byt) >> i);
            res_byte |= bit;
            res_len += 1;
            auto it = codes.find(std::make_pair(res_byte, res_len));
            if (it != codes.end()) {
                out.write_byte(it -> second);
                res_len = 0;
                res_byte = 0;
            }
            res_byte <<= 1;
        }
    }
}

void decode(std::ifstream & fin, std::ofstream & fout) {
    My_reader in(fin, 1);
    if(in.is_empty()) {
        return;
    }
    decode_map codes;
    codes = in.read_table();
    My_writer out(fout);
    write_decode(in, out, codes);
}
