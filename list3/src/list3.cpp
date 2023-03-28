#include <gtest/gtest.h>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>

struct Node
{
    int32_t value;
    int32_t leftCount;
    Node *left;
    Node *right;

    Node(int32_t value, int32_t leftCount = 0, Node *left = nullptr, Node *right = nullptr)
        : value(value), leftCount(leftCount), left(left), right(right) {}
};

namespace ex1
{
    void insertionSort(int32_t t[], size_t n)
    {
        int32_t sentinel = std::numeric_limits<int32_t>::min();
        t[-1] = sentinel;

        for (size_t i = 1; i < n; ++i)
        {
            int32_t x = t[i];
            size_t j = i - 1;
            while (t[j] > x)
            {
                t[j + 1] = t[j];
                --j;
            }
            t[j + 1] = x;
        }
    }

    TEST(List3_Exercise1, insertionSort)
    {
        int32_t t[] = {5, 1, 3, 2, 4};
        insertionSort(t, 5);
        for (int32_t i = 1; i <= 5; ++i)
            EXPECT_EQ(t[i - 1], i);

        int32_t t2[] = {5, 4, 3, 2, 1};
        insertionSort(t2, 5);
        for (int32_t i = 1; i <= 5; ++i)
            EXPECT_EQ(t2[i - 1], i);

        int32_t t3[] = {1, 2, 3, 4, 5};
        insertionSort(t3, 5);
        for (int32_t i = 1; i <= 5; ++i)
            EXPECT_EQ(t3[i - 1], i);

        int32_t t4[] = {1, 2, 3, 4};
        insertionSort(t4, 4);
        for (int32_t i = 1; i <= 4; ++i)
            EXPECT_EQ(t4[i - 1], i);

        int32_t t5[] = {2, 1};
        insertionSort(t5, 2);
        for (int32_t i = 1; i <= 2; ++i)
            EXPECT_EQ(t5[i - 1], i);

        int32_t t6[] = {3, 2, 1};
        insertionSort(t6, 3);
        for (int32_t i = 1; i <= 3; ++i)
            EXPECT_EQ(t6[i - 1], i);
    }
}

namespace ex2
{
    struct Node
    {
        int32_t value;
        Node *next;
        Node(int32_t value, Node *next = nullptr)
            : value(value), next(next) {}
        Node(std::vector<int32_t> values)
            : value(values[0]), next(nullptr)
        {
            Node *current = this;
            for (size_t i = 1; i < values.size(); ++i)
            {
                current->next = new Node(values[i]);
                current = current->next;
            }
        }
    };

    void reverse(Node *&head)
    {
        Node *previous = nullptr;
        while (head)
        {
            Node *next = head->next;
            head->next = previous;
            previous = head;
            head = next;
        }
        head = previous;
    }

    void insertionSort(Node *&head)
    {
        Node *sortedHead = nullptr;
        Node *current = head;

        while (current)
        {
            Node **sorted = &sortedHead;
            while (*sorted && (*sorted)->value > current->value)
                sorted = &(*sorted)->next;

            Node *next = current->next;
            if (*sorted)
            {
                Node *sortedNext = *sorted;
                *sorted = current;
                (*sorted)->next = sortedNext;
            }
            else
            {
                *sorted = current;
                (*sorted)->next = nullptr;
            }
            current = next;
        }

        reverse(sortedHead);
        head = sortedHead;
    }

    TEST(List3_Exercise2, insertionSort)
    {
        Node *head = new Node(1);
        insertionSort(head);
        EXPECT_EQ(head->value, 1);

        head = new Node({5, 4, 3, 2, 1});
        insertionSort(head);
        for (int32_t i = 1; i <= 5; ++i)
        {
            EXPECT_EQ(head->value, i);
            head = head->next;
        }

        head = new Node({3, 5, 1, 2, 4});
        insertionSort(head);
        for (int32_t i = 1; i <= 5; ++i)
        {
            EXPECT_EQ(head->value, i);
            head = head->next;
        }

        head = new Node({1, 2, 3, 4, 5});
        insertionSort(head);
        for (int32_t i = 1; i <= 5; ++i)
        {
            EXPECT_EQ(head->value, i);
            head = head->next;
        }
    }
}

namespace ex6
{
    Node *nth(Node *node, int32_t i)
    {
        if (i < 0)
            return nullptr;

        while (node)
        {
            if (node->leftCount == i)
                return node;
            else if (node->leftCount > i)
                node = node->left;
            else
            {
                i -= node->leftCount + 1;
                node = node->right;
            }
        }

        return nullptr;
    }

    TEST(List3_Exercise6, nth)
    {
        Node *root = new Node{4, 4};
        root->left = new Node{1, 1};
        root->left->left = new Node{0, 0};
        root->left->right = new Node{3, 1};
        root->left->right->left = new Node{2, 0};
        root->right = new Node{5, 0};
        root->right->right = new Node{7, 1};
        root->right->right->left = new Node{6, 0};

        EXPECT_EQ(nth(root, -1), nullptr);
        EXPECT_EQ(nth(root, 0)->value, 0);
        EXPECT_EQ(nth(root, 1)->value, 1);
        EXPECT_EQ(nth(root, 2)->value, 2);
        EXPECT_EQ(nth(root, 3)->value, 3);
        EXPECT_EQ(nth(root, 4)->value, 4);
        EXPECT_EQ(nth(root, 5)->value, 5);
        EXPECT_EQ(nth(root, 6)->value, 6);
        EXPECT_EQ(nth(root, 7)->value, 7);
        EXPECT_EQ(nth(root, 8), nullptr);
    }
}

namespace ex7
{
    void insert(Node *&node, int32_t value)
    {
        if (!node)
            node = new Node{value, 0};
        else
        {
            if (value < node->value)
            {
                ++node->leftCount;
                insert(node->left, value);
            }
            else
                insert(node->right, value);
        }
    }

    TEST(List3_Exercise7, insert)
    {
        Node *root = nullptr;
        insert(root, 4);
        insert(root, 1);
        insert(root, 0);
        insert(root, 3);
        insert(root, 2);
        insert(root, 5);
        insert(root, 7);
        insert(root, 6);

        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 4);
        EXPECT_EQ(root->left->value, 1);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->left->left->value, 0);
        EXPECT_EQ(root->left->left->leftCount, 0);
        EXPECT_EQ(root->left->right->value, 3);
        EXPECT_EQ(root->left->right->leftCount, 1);
        EXPECT_EQ(root->left->right->left->value, 2);
        EXPECT_EQ(root->left->right->left->leftCount, 0);
        EXPECT_EQ(root->right->value, 5);
        EXPECT_EQ(root->right->leftCount, 0);
        EXPECT_EQ(root->right->right->value, 7);
        EXPECT_EQ(root->right->right->leftCount, 1);
        EXPECT_EQ(root->right->right->left->value, 6);
        EXPECT_EQ(root->right->right->left->leftCount, 0);
    }
}

namespace ex8
{
    using ex7::insert;

    void remove(Node *&root, int32_t value)
    {
        Node **currentRoot = &root;
        std::stack<Node *> stack;
        while (*currentRoot && (*currentRoot)->value != value)
            if (value > (*currentRoot)->value)
                currentRoot = &(*currentRoot)->right;
            else
            {
                stack.push(*currentRoot);
                currentRoot = &(*currentRoot)->left;
            }

        if (*currentRoot == nullptr)
            return;

        if ((*currentRoot)->left && (*currentRoot)->right)
        {
            Node **min = &(*currentRoot)->right;
            while ((*min)->left)
            {
                stack.push(*min);
                min = &(*min)->left;
            }

            (*currentRoot)->value = (*min)->value;
            currentRoot = min;
        }

        while (!stack.empty())
        {
            --stack.top()->leftCount;
            stack.pop();
        }

        Node *child = (*currentRoot)->left ? (*currentRoot)->left : (*currentRoot)->right;
        delete *currentRoot;
        *currentRoot = child;
    }

    void removeNth(Node *&root, int32_t value)
    {
        if (value < 0)
            return;

        Node **currentRoot = &root;
        while (*currentRoot && (*currentRoot)->leftCount != value)
            if (value > (*currentRoot)->leftCount)
            {
                value -= (*currentRoot)->leftCount + 1;
                currentRoot = &(*currentRoot)->right;
            }
            else
            {
                --(*currentRoot)->leftCount;
                currentRoot = &(*currentRoot)->left;
            }

        if (*currentRoot == nullptr)
            return;

        if ((*currentRoot)->left && (*currentRoot)->right)
        {
            Node **min = &(*currentRoot)->right;
            while ((*min)->left)
            {
                --(*min)->leftCount;
                min = &(*min)->left;
            }

            (*currentRoot)->value = (*min)->value;
            currentRoot = min;
        }

        Node *child = (*currentRoot)->left ? (*currentRoot)->left : (*currentRoot)->right;
        delete *currentRoot;
        *currentRoot = child;
    }

    TEST(List3_Exercise8, remove)
    {
        Node *root = nullptr;
        insert(root, 3);
        insert(root, 1);
        insert(root, 0);
        insert(root, 2);
        insert(root, 4);
        insert(root, 5);

        remove(root, 3);
        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 3);
        EXPECT_EQ(root->left->value, 1);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->right->value, 5);
        EXPECT_EQ(root->right->leftCount, 0);

        remove(root, 1);
        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 2);
        EXPECT_EQ(root->left->value, 2);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->left->left->value, 0);
        EXPECT_EQ(root->left->left->leftCount, 0);
        EXPECT_EQ(root->right->value, 5);
        EXPECT_EQ(root->right->leftCount, 0);

        remove(root, -1);
        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 2);
        EXPECT_EQ(root->left->value, 2);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->left->left->value, 0);
        EXPECT_EQ(root->left->left->leftCount, 0);
        EXPECT_EQ(root->right->value, 5);
        EXPECT_EQ(root->right->leftCount, 0);

        root = nullptr;
        insert(root, 0);
        insert(root, 3);
        insert(root, 2);

        remove(root, 1);
        EXPECT_EQ(root->leftCount, 0);
        EXPECT_EQ(root->right->leftCount, 1);
        EXPECT_EQ(root->right->left->leftCount, 0);
    }

    TEST(List3_Exercise8, removeNth)
    {
        Node *root = nullptr;
        insert(root, 3);
        insert(root, 1);
        insert(root, 0);
        insert(root, 2);
        insert(root, 4);
        insert(root, 6);
        insert(root, 5);

        removeNth(root, 3);
        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 3);
        EXPECT_EQ(root->left->value, 1);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->right->value, 6);
        EXPECT_EQ(root->right->leftCount, 1);

        removeNth(root, 1);
        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 2);
        EXPECT_EQ(root->left->value, 2);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->left->left->value, 0);
        EXPECT_EQ(root->left->left->leftCount, 0);

        removeNth(root, 4);
        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 2);
        EXPECT_EQ(root->left->value, 2);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->right->value, 5);
        EXPECT_EQ(root->right->leftCount, 0);

        removeNth(root, 7);
        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 2);
        EXPECT_EQ(root->left->value, 2);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->right->value, 5);
        EXPECT_EQ(root->right->leftCount, 0);

        removeNth(root, -1);
        EXPECT_EQ(root->value, 4);
        EXPECT_EQ(root->leftCount, 2);
        EXPECT_EQ(root->left->value, 2);
        EXPECT_EQ(root->left->leftCount, 1);
        EXPECT_EQ(root->right->value, 5);
        EXPECT_EQ(root->right->leftCount, 0);
    }
}