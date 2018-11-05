#include <iostream>
#include <string>

#include "tree.h"
#include "my_reader.h"

Node::Node() : cnt(0), c(0) {}

Node::Node(uint64_t _cnt, uint8_t _c) : cnt(_cnt), c(_c) {}

Node::Node(sp _L, sp _R) : L(_L), R(_R) {
    set_cnt(_L->get_cnt() + _R->get_cnt());
}

Node::~Node () {
    L = R = nullptr;
}

bool Comp::operator() (const sp & a, const sp & b) const {
    return a->get_cnt() < b->get_cnt();
}

void Node::DFS(uint64_t s, uint64_t len, std::vector < std::pair<uint64_t, uint64_t> > & codes) {
    //std::cout << "DFS : " << s << " " << len << "\n";
    if (L == nullptr && R == nullptr) {
        if (!len) {
            len = 1;
        }
        code = s;
        code_len = len;
        codes[c] = {s, len};
      //  std::cout << c << " " << s << " " << len << "\n" << codes[c].first << " " << codes[c].second << "\n";
        return;
    }

    L->DFS(s << 1, len + 1,  codes);
    R->DFS( (s << 1) + 1, len + 1, codes);
}

void make_tree(std::vector <uint64_t> & num, huffman_table & codes) {
    std::multiset<sp, Comp> ms;
//    std::cout << "XUI " << ((size_t)0 < ALPHA) << "\n";
//    std::cout << num.size() << "\n";
    for (size_t i = 0; i < ALPHA; ++i) {
        //std::cout << i << " : ";
        if (num[i] != 0u) {
        //    std::cout << "->>>>>>>>>>>>>>>>>> " << i << " " << num[i] << "\n";
            ms.insert(std::make_shared <Node>(Node(num[i], i)));
        }
        //std::cout << "\n";
    }
    //std::cout << "HEY\n";
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
