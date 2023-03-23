#include <gtest/gtest.h>
#include <iostream>
#include <numeric>
#include <vector>

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
    struct Node
    {
        int32_t value;
        int32_t leftCount;
        Node *left;
        Node *right;

        Node(int32_t value, int32_t leftCount = 0, Node *left = nullptr, Node *right = nullptr)
            : value(value), leftCount(leftCount), left(left), right(right) {}
    };

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