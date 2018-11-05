#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include "../gtest/gtest.h"
#include <ctime>
#include <string>
#include <iostream>

#include "../lib/huffman.h"

const std::string FILE_NAME = "file";
const std::string IN = ".in";
const std::string OUT = ".out";
const std::string TXT = ".txt";

std::string encode_and_decode(std::string const& str, bool fl = 0) {
    if (!fl) {
        std::ofstream out(FILE_NAME + IN);
        out << str;
        out.close();
    }

    std::ifstream fin(FILE_NAME + IN);
    std::ofstream fout(FILE_NAME + TXT);
    encode(fin, fout);
    fin.close();
    fout.close();

    std::ifstream fi(FILE_NAME + TXT);
    std::ofstream fo(FILE_NAME + OUT);
    decode(fi, fo);
    fi.close();
    fo.close();

    std::ifstream in(FILE_NAME + OUT);
    std::string res;
    getline(in, res);
    in.close();
    return res;
    return "";
} 

bool equal_files(std::ifstream & in1, std::ifstream & in2) {
    uint32_t size1;
    uint32_t size2;

    size1 = in1.seekg(0, in1.end).tellg();
    in1.seekg(0, in1.beg);

    size2 = in2.seekg(0, in2.end).tellg();
    in2.seekg(0, in2.beg);

    if(size1 != size2) {
        return false;
    }

    while (size1) {
        char buf1[BLOCK_SIZE];
        char buf2[BLOCK_SIZE];
        uint32_t read_size = BLOCK_SIZE > size1 ? size1 : BLOCK_SIZE;

        in1.read(buf1, read_size);
        in2.read(buf2, read_size);

        if(memcmp(buf1, buf2, read_size ) != 0) {
            return false;
        }

        size1 -= read_size ;
    }

    return true;
}

TEST(correctness, empty_test) {
    std::string test = "";

    EXPECT_EQ(test, encode_and_decode(test));
}


TEST(correctness, digits_test) {
    std::string test = "123";

    EXPECT_EQ(test, encode_and_decode(test));
}


TEST(correctness, space_test) {
    std::string test = "     ";

    EXPECT_EQ(test, encode_and_decode(test));
}

TEST(correctness, one_symbol_test) {
    std::string test = "a";

    EXPECT_EQ(test, encode_and_decode(test));
}

TEST(correctness, russian_symbols_test) {
    std::string test = "балалайка матрешка водка";

    EXPECT_EQ(test, encode_and_decode(test));
}

TEST(correctness, special_symbols_test) {
    std::string test = "!@#$%^&*()_+/><;:\"\'\\ .?,";

    EXPECT_EQ(test, encode_and_decode(test));
}


TEST(correctness, enter_test) {
    std::string test = "\n\n\n\n\n\n";
    encode_and_decode(test);

    std::ifstream f1(FILE_NAME + IN);
    std::ifstream f2(FILE_NAME + OUT);
    EXPECT_TRUE(equal_files(f1, f2));
}

TEST(correctness, all_ASCII_test) {
    std::string test = "";
    for (int i = 0; i < 256; ++i) {
        test += (char) i;
    }

    encode_and_decode(test);

    std::ifstream f1(FILE_NAME + IN);
    std::ifstream f2(FILE_NAME + OUT);
    EXPECT_TRUE(equal_files(f1, f2));
}


TEST(correctness, random_test) {
    srand(time(0));
    for (size_t i = 0; i < 10; ++i) {
        std::ofstream out(FILE_NAME + IN);
        uint32_t sz = rand() % 4000;
        for (size_t i = 0; i < sz; ++i) {
            out << char(rand() % 20);

        }
        out.close();

        encode_and_decode("", 1);

        std::ifstream f1(FILE_NAME + IN);
        std::ifstream f2(FILE_NAME + OUT);
        EXPECT_TRUE(equal_files(f1, f2));
    }
}

TEST(correctness, big_abc_dig_test) {
    std::ofstream out(FILE_NAME + IN);
    std::vector<char> v;
    for (char i = 'a'; i <= 'z'; ++i) {
        v.push_back(i);
    }
    for (char i = '1'; i <= '9'; ++i) {
        v.push_back(i);
    }
    srand(time(0));
    for (size_t i = 0; i < 100000; ++i) {
        out << v[rand() % 35];
    }
    out.close();

    encode_and_decode("", 1);

    std::ifstream f1(FILE_NAME + IN);
    std::ifstream f2(FILE_NAME + OUT);
    EXPECT_TRUE(equal_files(f1, f2));
}

TEST(correctness, 15mb_test) {
    std::ifstream fin("test/big_file.in");
    std::ofstream fout(FILE_NAME + TXT);
    encode(fin, fout);
    fin.close();
    fout.close();

    std::ifstream fi(FILE_NAME + TXT);
    std::ofstream fo(FILE_NAME + OUT);
    decode(fi, fo);
    fi.close();
    fo.close();

    std::ifstream f1("test/big_file.in");
    std::ifstream f2(FILE_NAME + OUT);
    EXPECT_TRUE(equal_files(f1, f2));
}

TEST(correctness, pdf_test) {
    std::ifstream fin("test/sample.pdf");
    std::ofstream fout(FILE_NAME + TXT);
    encode(fin, fout);
    fin.close();
    fout.close();

    std::ifstream fi(FILE_NAME + TXT);
    std::ofstream fo(FILE_NAME + OUT);
    decode(fi, fo);
    fi.close();
    fo.close();


    std::ifstream f1("test/sample.pdf");
    std::ifstream f2(FILE_NAME + OUT);
    EXPECT_TRUE(equal_files(f1, f2));
}

