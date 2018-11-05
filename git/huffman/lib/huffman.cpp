#include <fstream>
#include <iostream>

#include "huffman.h"

void build_table(My_reader & in, VEC <uint64_t> & num) {
    while (!in.is_end()) {
        uint8_t byt = in.read_byte();
        //std:: cout << byt << "!!!\n";
        ++num[byt];
    }
}

/*
void logging(uint8_t c, size_t len) {
    std::string s = "";
    for (int i = 7; i >= 0; --i) {
        s += (((c & ((uint8_t)1 << i)) > 0) + '0');
    }
    std::cout << "RES_BYTE " << s << " " << len << "\n";
}

void smp_logging(uint8_t c) {
    std::string s = "";
    for (int i = 7; i >= 0; --i) {
        s += (((c & ((uint8_t)1 << i)) > 0) + '0');
    }
    std::cout << s << "\n";
}
*/

void write_encode(My_reader & in, My_writer & out, huffman_table & codes) {
    //std::cout << "BEFORE to_begin\n";
    in.to_begin();
    out.write_table(codes); // выводим таблицу Хаффмана

    uint8_t ost = 8,
            res_byte = 0,
            pos_in_byte = 0;
    while (!in.is_end()) { // формируем группы по 4кб и отправляем на вывод
        uint8_t byt = in.read_byte();
        auto it = codes[byt];
        //smp_logging(byt);
        //std::cout << "BYT : " << byt << " " << it.FF << " " << it.SS << "\n";

        while (it.SS >= ost) {
            //std::cout << it.SS << " " << (int)ost << "\n";
            size_t rub = (64 - it.SS);
            it.FF <<= rub >>= rub;
            res_byte |= (it.FF >> (it.SS - ost));
            //logging(out.buf[out.pos_in_byte], out.pos_in_byte_in_byte);
            //std::cout << "OUT BUF ";
            //smp_logging(out.buf[out.pos_in_byte]);
            out.push(res_byte);
            res_byte = 0;
            it.SS -= ost;
            ost = 8;
            pos_in_byte = 0;
        }
        if (it.SS < ost) {
            //std::cout << "IF : " << it.SS << " " << (int)ost << "\n";
            size_t rub = (64 - it.SS);
            it.FF <<= rub >>= rub;
            res_byte |= (it.FF << (ost - it.SS));
            //std::cout << "OUT BUF ";
            //smp_logging(out.buf[out.pos_in_byte]);
            pos_in_byte += it.SS;
            ost -= it.SS;
            it.SS = 0;
        }
    }
    //std::cout << "AFTER\n";
    //logging(out.buf[out.pos_in_byte], out.pos_in_byte_in_byte);
    if (pos_in_byte) {
        out.push(res_byte);
    }
    out.write_buf();
    ost %= 8;
    //smp_logging(ost);
    out.write_ost(ost); // выведем, сколько лишних битов-нулей записано в файл
}

void encode(std::istream & fin, std::ostream & fout) {
    //std::cout << "BECAME\n";
    My_reader in(fin);
    My_writer out(fout);
    VEC <uint64_t> num(ALPHA);

    huffman_table codes(ALPHA);
    build_table(in, num);
//    for (size_t i = 0; i < 256; ++i) {
//        std::cout << num[i] << " ";
//    }

    if (!in.get_file_size()) {
    //    std::cout << "empty!\n";
        return;
    }
    //std:: cout << "BEFORE make_tree\n";
    make_tree(num, codes);
    // std::cout << "AFTER make_tree\n";
    write_encode(in, out, codes);
    //std::cout << "GONE\n";
}

void write_decode(My_reader & in, My_writer & out, decode_map & codes) { //формируем блоки по 4кб и отправляем на вывод
    in.read_next_section();
    //std::cout << "START " << in.read_size << " " << in.pos << "\n";
    uint64_t res_byte = 0,
             res_len = 0    ;
    int fl = 0;
    while (!in.is_end()) {
        uint8_t byt = in.read_byte();
        if (in.get_last_symbol()) {
            continue;
        }
        //std::cout << "BYT : ";
        //smp_logging(byt);
        //std::cout << "CHECK : " << in.file_size << " " << in.read_size << " " << in.pos  << "\n";
        if (in.is_zeroes_cnt()) {
            fl = in.get_zeroes_cnt();
           // std::cout << "FL " << fl << "\n";
        }
        for (int i = 7; i >= fl; --i) {
            uint8_t bit = (((1 << i) & byt) >> i);
            res_byte |= bit;
            res_len += 1;
            auto it = codes.find(std::make_pair(res_byte, res_len));
            if (it != codes.end()) {
                out.push(it->SS);
                res_len = 0;
                res_byte = 0;
            }
            res_byte <<= 1;
        }
    }
    out.write_buf();
}

void decode(std::istream & fin, std::ostream & fout) {
    My_reader in(fin, 1);
    if(in.get_file_size() == 0) {
        return;
    }
    decode_map codes;
    codes = in.read_table();
//    for (auto it : codes) {
//        smp_logging(it.FF.FF);
//        std::cout << " " << it.FF.SS << " " << it.SS << "\n";
//    }
    My_writer out(fout);
    write_decode(in, out, codes);
}
