#ifndef TREE_H
#define TREE_H

#include <memory>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

#include "my_reader.h"
#include "constants.h"

struct Node {
    Node();
    Node(uint64_t cnt, uint8_t c);
    Node(std::shared_ptr <Node>, std::shared_ptr <Node>);

    ~Node ();

    uint64_t get_cnt() const {
        return cnt;
    }

    void set_cnt(uint64_t new_cnt) {
        cnt = new_cnt;
    }

    void DFS(uint64_t, uint64_t, huffman_table &);

    private:
    std::shared_ptr <Node> L = nullptr;
    std::shared_ptr <Node> R = nullptr;
    uint64_t cnt = 0;
    uint64_t code_len = 0;
    uint8_t c = 0;
    uint64_t code = 0;
};

struct Comp {
    bool operator() (const std::shared_ptr <Node> &, const std::shared_ptr <Node> &) const;
};

void make_tree(std::vector <uint64_t> &, huffman_table &);

#endif // TREE_H
