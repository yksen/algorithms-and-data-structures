#include <gtest/gtest.h>

namespace ex1
{
    struct NodeBST
    {
        int32_t value;
        NodeBST* left;
        NodeBST* right;  
    };

    struct NodeRBT : public NodeBST
    {
        bool isBlack;
    };
}