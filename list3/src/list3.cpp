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

    void insertionSort(Node *&head)
    {
        
    }

    TEST(List3_Exercise2, insertionSort)
    {
        Node *head = new Node({3, 5, 1, 2, 4});
        insertionSort(head);

        for (int32_t i = 1; i <= 5; ++i)
        {
            EXPECT_EQ(head->value, i);
            head = head->next;
        }

        
    }
}