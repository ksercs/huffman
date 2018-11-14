#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>

#include "lib/huffman.h"

const std::string EN = "encode";
const std::string DE = "decode";

// input format:
// 1 string (com)         = 'encode' or 'decode'
// 2 string (input_name)  = name of input file
// 3 string (output_name) = name of output file
int main(int argc, char * argv[]) {
    std::string com, input_name, output_name;
    //std::cin >> com >> input_name >> output_name;
    if (argc != 4) {
        std::cout << "encode/decode input_file output_file\n";
        return 0;
    }
    if (check_endian()) {
        std::cout << "Sorry, only for little endian\n";
        return 0;
    }
    com = argv[1];
    input_name = argv[2];
    output_name = argv[3];

    if (com != EN && com != DE) {
        std::cout << "Invalid command!\n";
    } else {
        std::ifstream fin(input_name);
        std::ofstream(output_name, std::ios::out);
        std::ofstream fout(output_name, std::ios::app);
        if (!fin) {
            std::cout << "Can't open input file!\n";
        } else if (!fout) {
            std::cout << "Can't open output file\n";
        } else {
            clock_t tm = clock();
            switch (com[0]) {
                case 'e' :
                    //std:: cout << "BEFORE\n";
                    encode(fin, fout);
                    //std::cout << "AFTER\n";
                    break;
                case 'd' :
                    try {
                        decode(fin, fout);
                    } catch (...) {
                        std::cout << "Invalid input file\n";
                    }
                    break;
                default:
                    break;
            }
            fin.close();
            fout.close();
            double res_tm = (clock() - tm) / CLOCKS_PER_SEC;
            std::cout.precision(2);
            std::cout << std::fixed << "OK! --->  " << res_tm << "\n";
        }
    }
    return 0;
}
