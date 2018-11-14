#include <iostream>
#include <string>

#include "tree.h"
#include "my_reader.h"

Node::Node() : cnt(0), c(0) {}

Node::Node(uint64_t _cnt, uint8_t _c) : cnt(_cnt), c(_c) {}

Node::Node(std::shared_ptr <Node> _L, std::shared_ptr <Node> _R) : L(_L), R(_R) {
    set_cnt(_L->get_cnt() + _R->get_cnt());
}

Node::~Node () {
    L = R = nullptr;
}

bool Comp::operator() (const std::shared_ptr <Node> & a, const std::shared_ptr <Node> & b) const {
    return a->get_cnt() < b->get_cnt();
}

void Node::DFS(uint64_t s, uint64_t len, std::vector < std::pair<uint64_t, uint64_t> > & codes) {
    if (L == nullptr && R == nullptr) {
        if (!len) {
            len = 1;
        }
        code = s;
        code_len = len;
        codes[c] = {s, len};
        return;
    }

    L->DFS(s << 1, len + 1,  codes);
    R->DFS( (s << 1) + 1, len + 1, codes);
}

void make_tree(std::vector <uint64_t> & num, huffman_table & codes) {
    std::multiset<std::shared_ptr <Node>, Comp> ms;
    for (size_t i = 0; i < ALPHA; ++i) {
        if (num[i] != 0u) {
            ms.insert(std::make_shared <Node>(Node(num[i], i)));
        }
    }
    while (ms.size() > 1) {
        auto a = *ms.begin();
        ms.erase(ms.begin());
        auto b = *ms.begin();
        ms.erase(ms.begin());
        ms.insert(std::make_shared <Node>(Node(a, b)));
    }

    auto root = *ms.begin();
    root->DFS(0, 0, codes);
}
