#include <gtest/gtest.h>

namespace ex8
{
    struct Node
    {
        int32_t value;
        Node *next;
        Node(int32_t value, Node *next = nullptr) : value(value), next(next) {}
        Node(std::vector<int32_t> values) : Node(values[0], values.size() > 1 ? new Node(std::vector<int32_t>(values.begin() + 1, values.end())) : nullptr) {}
    };

    void countingSort(Node *&head, uint32_t max)
    {
        std::vector<Node *> queues(max, nullptr);

        while (head)
        {
            Node *node = head;
            head = head->next;

            Node **queue = &queues[node->value];
            node->next = *queue;
            *queue = node;
        }

        Node **queue = &head;
        for (Node *node : queues)
        {
            *queue = node;
            while (*queue)
                queue = &(*queue)->next;
        }
    }

    TEST(List5_Exercise8, countingSort)
    {
        Node *head = new Node({1, 2, 3, 3, 2, 1, 2, 1, 3});
        countingSort(head, 4);
        for (uint32_t i = 1; i <= 3; ++i)
            for (size_t j = 0; j < 3; ++j)
            {
                ASSERT_NE(head, nullptr);
                EXPECT_EQ(head->value, i);
                head = head->next;
            }
        EXPECT_EQ(head, nullptr);

        head = new Node({3, 1, 2});
        countingSort(head, 4);
        for (uint32_t i = 1; i <= 3; ++i)
        {
            ASSERT_NE(head, nullptr);
            EXPECT_EQ(head->value, i);
            head = head->next;
        }
        EXPECT_EQ(head, nullptr);

        head = new Node({1, 1, 1, 1});
        countingSort(head, 2);
        for (uint32_t i = 1; i <= 4; ++i)
        {
            ASSERT_NE(head, nullptr);
            EXPECT_EQ(head->value, 1);
            head = head->next;
        }
        EXPECT_EQ(head, nullptr);

        head = new Node({9, 0, 5});
        countingSort(head, 10);
        EXPECT_EQ(head->value, 0);
        EXPECT_EQ(head->next->value, 5);
        EXPECT_EQ(head->next->next->value, 9);
    }
}

namespace ex9
{
    int32_t partition(int32_t t[], int32_t n)
    {
        int32_t k = -1;
        int32_t x = t[n / 2];
        for (;;)
        {
            do
                ++k;
            while (t[k] < x);
            do
                --n;
            while (t[n] > x);
            if (k < n)
                std::swap(t[k], t[n]);
            else
                return k;
        }
    }

    int32_t kth(int32_t t[], int32_t n, int32_t k)
    {
        if (n == 1)
            return t[0];
        int32_t p = partition(t, n);
        if (p == k)
            return t[p];
        else if (p > k)
            return kth(t, p, k);
        else
            return kth(t + p + 1, n - p - 1, k - p - 1);
    }

    TEST(List5_Exercise9, kth)
    {
        int32_t t1[] = {1, 2, 3};
        for (int32_t i = 0; i < 3; i++)
            ASSERT_EQ(kth(t1, 3, i), i + 1);

        int32_t t2[] = {6, 3, 5, 1, 9, 8, 7, 2, 4};
        for (int32_t i = 0; i < 9; i++)
            ASSERT_EQ(kth(t2, 9, i), i + 1);

        int32_t t3[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int32_t i = 0; i < 9; i++)
            ASSERT_EQ(kth(t3, 9, i), i + 1);

        int32_t t4[] = {-1, 0, 1};
        for (int32_t i = 0; i < 3; i++)
            ASSERT_EQ(kth(t4, 3, i), i - 1);

        int32_t t5[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        for (int32_t i = 0; i < 9; i++)
            ASSERT_EQ(kth(t5, 9, i), 1);
    }
}