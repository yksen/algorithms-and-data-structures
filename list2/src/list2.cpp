#include <gtest/gtest.h>
#include <iostream>
#include <stack>

struct Node
{
    int32_t key;
    Node *left;
    Node *right;
    Node *parent;

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

    void remove(Node *&root, int32_t key)
    {
        Node **current_root = &root;
        while (*current_root && (**current_root).key != key)
            current_root = key > (**current_root).key ? &(**current_root).right : &(**current_root).left;

        if (*current_root == nullptr)
            return;

        if ((**current_root).left && (**current_root).right)
        {
            Node **min = &(**current_root).right;
            while ((**min).left)
                min = &(**min).left;

            (**current_root).key = (**min).key;
            current_root = min;
        }

        Node *child = (**current_root).left ? (**current_root).left : (**current_root).right;
        if (child)
            child->parent = (**current_root).parent;
        delete *current_root;
        *current_root = child;
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

    TEST(Exercise1, remove)
    {
        Node *root = nullptr;
        insert(root, 5);
        insert(root, 3);
        insert(root, 7);

        remove(root, 3);

        EXPECT_EQ(root->key, 5);
        EXPECT_EQ(root->left, nullptr);
        EXPECT_EQ(root->right->key, 7);
        EXPECT_EQ(root->parent, nullptr);
        EXPECT_EQ(root->right->parent, root);

        remove(root, 5);

        EXPECT_EQ(root->key, 7);
        EXPECT_EQ(root->parent, nullptr);

        remove(root, 7);

        EXPECT_EQ(root, nullptr);

        insert(root, 1);
        insert(root, 3);
        insert(root, 2);
        insert(root, 4);

        remove(root, 3);

        EXPECT_EQ(root->key, 1);
        EXPECT_EQ(root->right->key, 4);
        EXPECT_EQ(root->right->left->key, 2);
        EXPECT_EQ(root->right->right, nullptr);
        EXPECT_EQ(root->parent, nullptr);
        EXPECT_EQ(root->right->parent, root);
    }
}

namespace ex2
{
    void inorderDo(Node *root, void (*f)(Node *))
    {
        if (root == nullptr)
            return;

        inorderDo(root->left, f);
        f(root);
        inorderDo(root->right, f);
    }

    TEST(Exercise2, inorderDo)
    {
        Node *root = nullptr;
        ex1::insert(root, 5);
        ex1::insert(root, 3);
        ex1::insert(root, 7);
        ex1::insert(root, 2);
        ex1::insert(root, 4);

        inorderDo(root, [](Node *node)
                  { std::cout << node->key << " "; });
        inorderDo(root, [](Node *node)
                  { node->key = 0; });

        EXPECT_EQ(root->key, 0);
        EXPECT_EQ(root->left->key, 0);
        EXPECT_EQ(root->right->key, 0);
        EXPECT_EQ(root->left->left->key, 0);
        EXPECT_EQ(root->left->right->key, 0);
    }
}

namespace ex3
{
    void inorderDo(Node *root, void (*f)(Node *))
    {
        std::stack<Node *> stack;
        Node *current = root;

        while (current != nullptr || !stack.empty())
        {
            while (current != nullptr)
            {
                stack.push(current);
                current = current->left;
            }

            current = stack.top();
            stack.pop();

            f(current);

            current = current->right;
        }
    }

    TEST(Exercise3, inorderDo)
    {
        Node *root = nullptr;
        ex1::insert(root, 3);
        ex1::insert(root, 2);
        ex1::insert(root, 1);
        ex1::insert(root, 4);
        ex1::insert(root, 5);

        inorderDo(root, [](Node *node)
                  { std::cout << node->key << " "; });
        inorderDo(root, [](Node *node)
                  { node->key++; });

        EXPECT_EQ(root->key, 4);
        EXPECT_EQ(root->left->key, 3);
        EXPECT_EQ(root->right->key, 5);
        EXPECT_EQ(root->left->left->key, 2);
        EXPECT_EQ(root->right->right->key, 6);
    }
}