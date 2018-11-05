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
#define sp  std::shared_ptr <Node>

const uint32_t BLOCK_SIZE = (4 * ( (1u) << 10 ));
const uint64_t ALPHA = (1u << 8);

typedef std::vector <std::pair <uint64_t, uint64_t> > huffman_table;
typedef std::map <std::pair<uint64_t, uint64_t>, uint8_t> decode_map;

#endif //CONSTANTS_H
