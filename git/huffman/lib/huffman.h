#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <fstream>
#include "my_reader.h"
#include "my_writer.h"
#include "tree.h"

void encode(std::istream &, std:: ostream &);
void decode(std::istream &, std:: ostream &);

void build_table(My_reader &);
void write_encode(My_reader &, My_writer &, huffman_table &);
void write_decode(My_reader &, My_writer &, decode_map &);

#endif //HUFFMAN_H
