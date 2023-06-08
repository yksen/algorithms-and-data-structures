#include <gtest/gtest.h>

namespace ex1
{
    struct BTree
    {
        uint32_t t;
        bool isLeaf;
        size_t keyCount;
        int32_t *keys;
        BTree **children;
    };
}