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

    TEST(List2_Exercise1, insert)
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

    TEST(List2_Exercise1, remove)
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

    TEST(List2_Exercise2, inorderDo)
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
        if (root == nullptr)
            return;

        Node *current = root;
        while (current->left)
            current = current->left;

        f(current);

        Node *next = current;
        while (next)
        {
            if (current->right)
            {
                next = current->right;
                while (next->left)
                    next = next->left;
            }
            else
            {
                next = current->parent;
                while (next && next->right == current)
                {
                    current = next;
                    next = next->parent;
                }
            }

            if (next)
                f(next);
            current = next;
        }
    }

    TEST(List2_Exercise3, inorderDo)
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

        root = nullptr;
        ex1::insert(root, 2);
        ex1::insert(root, 4);
        ex1::insert(root, 3);

        inorderDo(root, [](Node *node)
                  { std::cout << node->key << " "; });
        inorderDo(root, [](Node *node)
                  { node->key++; });

        EXPECT_EQ(root->key, 3);
        EXPECT_EQ(root->right->key, 5);
        EXPECT_EQ(root->right->left->key, 4);
    }
}

namespace ex4
{
    struct Node
    {
        int32_t key;
        Node *left;
        Node *right;
        Node *parent;

        Node(int32_t key, Node *parent) : key(key), left(nullptr), right(nullptr), parent(parent) {}

        struct iterator
        {
            iterator(Node *current) : current(current) {}

            int32_t &operator*() { return current->key; }
            iterator &operator++()
            {
                if (current->right)
                {
                    current = current->right;
                    while (current->left)
                        current = current->left;
                }
                else
                {
                    Node *parent = current->parent;
                    while (parent && current == parent->right)
                    {
                        current = parent;
                        parent = parent->parent;
                    }
                    current = parent;
                }
                return *this;
            }
            bool operator!=(const iterator &other) { return current != other.current; }

        private:
            Node *current;
        };

        iterator begin()
        {
            Node *current = this;
            while (current->left)
                current = current->left;
            return iterator(current);
        }

        iterator end() { return iterator(nullptr); }
    };

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

    TEST(List2_Exercise4, BSTiterator)
    {
        Node *root = nullptr;
        insert(root, 3);
        insert(root, 5);
        insert(root, 1);
        insert(root, 2);
        insert(root, 4);

        int32_t count = 1;
        for (Node::iterator it = root->begin(); it != root->end(); ++it)
        {
            std::cout << *it << " ";
            EXPECT_EQ(*it, count++);
        }
        EXPECT_EQ(count, 6);

        count = 1;
        for (auto x : *root)
        {
            std::cout << x << " ";
            EXPECT_EQ(x, count++);
        }
        EXPECT_EQ(count, 6);
    }
}

namespace ex5
{
    template <typename T>
    struct Node
    {
        T key;
        Node<T> *left;
        Node<T> *right;

        Node<T>(T key) : key(key), left(nullptr), right(nullptr) {}

        struct iterator
        {
            iterator(Node<T> *root) : current(root)
            {
                while (current)
                {
                    stack.push(current);
                    current = current->left;
                }
                if (!stack.empty())
                {
                    current = stack.top();
                    stack.pop();
                }
            }

            T &operator*() { return current->key; }
            iterator &operator++()
            {
                if (current->right)
                {
                    current = current->right;
                    while (current)
                    {
                        stack.push(current);
                        current = current->left;
                    }
                }
                if (!stack.empty())
                {
                    current = stack.top();
                    stack.pop();
                }
                else
                    current = nullptr;

                return *this;
            }
            bool operator!=(const iterator &other) { return current != other.current; }

        private:
            Node<T> *current;
            std::stack<Node<T> *> stack;
        };

        iterator begin() { return iterator(this); }
        iterator end() { return iterator(nullptr); }
    };

    template <typename T>
    void insert(Node<T> *&root, T key)
    {
        Node<T> **current_root = &root;

        while (*current_root != nullptr)
        {
            if (key < (*current_root)->key)
                current_root = &(*current_root)->left;
            else
                current_root = &(*current_root)->right;
        }

        *current_root = new Node<T>(key);
    }

    TEST(List2_Exercise5, BSTiterator)
    {
        Node<double> *root = nullptr;
        insert(root, 3.5);
        insert(root, 5.5);
        insert(root, 1.5);
        insert(root, 2.5);
        insert(root, 4.5);

        double count = 1.5;
        for (Node<double>::iterator it = root->begin(); it != root->end(); ++it)
        {
            std::cout << *it << " ";
            EXPECT_DOUBLE_EQ(*it, count++);
        }
        EXPECT_DOUBLE_EQ(count, 6.5);

        count = 1.5;
        for (auto x : *root)
        {
            std::cout << x << " ";
            EXPECT_DOUBLE_EQ(x, count++);
        }
        EXPECT_DOUBLE_EQ(count, 6.5);
    }
}