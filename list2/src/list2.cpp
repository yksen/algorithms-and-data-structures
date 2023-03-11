#include <gtest/gtest.h>
#include <iostream>

struct Node
{
    int32_t key;
    Node *left;
    Node *right;
    Node *parent;

    Node(int32_t key) : key(key), left(nullptr), right(nullptr), parent(nullptr) {}
    Node(int32_t key, Node *parent) : key(key), left(nullptr), right(nullptr), parent(parent) {}
};

namespace ex1
{
    void insert(Node *&root, int32_t key)
    {
        Node **current_root = &root;
        Node *last_root = nullptr;

        while (*current_root != nullptr)
        {
            last_root = *current_root;
            if (key < (*current_root)->key)
                current_root = &(*current_root)->left;
            else
                current_root = &(*current_root)->right;
        }

        *current_root = new Node(key, last_root);
    }

    TEST(Exercise1, insert)
    {
        Node *root = nullptr;
        insert(root, 5);
        insert(root, 3);
        insert(root, 7);
        insert(root, 2);
        insert(root, 4);

        EXPECT_EQ(root->key, 5);
        EXPECT_EQ(root->left->key, 3);
        EXPECT_EQ(root->right->key, 7);
        EXPECT_EQ(root->left->left->key, 2);
        EXPECT_EQ(root->left->right->key, 4);

        EXPECT_EQ(root->parent, nullptr);
        EXPECT_EQ(root->left->parent->key, 5);
        EXPECT_EQ(root->right->parent->key, 5);
        EXPECT_EQ(root->left->left->parent->key, 3);
        EXPECT_EQ(root->left->right->parent->key, 3);
    }
}