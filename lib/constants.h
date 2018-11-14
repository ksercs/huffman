#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <memory>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>

#define VEC std::vector
#define SS second
#define FF first

const uint32_t BUF_SIZE = 3;
const uint32_t BLOCK_SIZE = 4 * (1u << 10);
const uint64_t ALPHA = (1u << 8);

using huffman_table = std::vector <std::pair <uint64_t, uint64_t> >;
using decode_map = std::map <std::pair<uint64_t, uint64_t>, uint8_t>;

#endif //CONSTANTS_H
