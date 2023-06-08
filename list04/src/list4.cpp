#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>
#include <random>

namespace ex9
{
    struct Node
    {
        int32_t value;
        Node *next;

        Node(int32_t value, Node *next = nullptr) : value(value), next(next) {}
        Node(std::vector<int32_t> values) : Node(values[0], values.size() > 1 ? new Node(std::vector<int32_t>(values.begin() + 1, values.end())) : nullptr) {}
    };

    Node *mergeSortedLists(std::vector<Node *> heads)
    {
        if (heads.empty())
            return nullptr;

        Node *merged = nullptr;
        std::vector<Node *> heap;
        for (auto head : heads)
            while (head != nullptr)
            {
                heap.push_back(head);
                std::push_heap(heap.begin(), heap.end(), [](Node *a, Node *b)
                               { return a->value < b->value; });
                head = head->next;
            }

        while (!heap.empty())
        {
            std::pop_heap(heap.begin(), heap.end(), [](Node *a, Node *b)
                          { return a->value < b->value; });
            Node *node = heap.back();
            heap.pop_back();
            node->next = merged;
            merged = node;
        }

        return merged;
    }

    TEST(List4_Exercise9, mergeSortedLists)
    {
        std::vector<Node *> heads{
            new Node(3),
            new Node(1),
            new Node(2),
        };
        Node *merged = mergeSortedLists(heads);
        for (int32_t i = 1; i <= 3; i++)
        {
            ASSERT_NE(merged, nullptr);
            EXPECT_EQ(merged->value, i);
            merged = merged->next;
        }
        EXPECT_EQ(merged, nullptr);

        heads = {
            new Node({1, 3, 5, 7, 9}),
            new Node({2, 4, 6, 8, 10}),
            new Node({11, 13, 15, 17, 19}),
            new Node({12, 14, 16, 18, 20}),
            new Node({21, 23, 25, 27, 29}),
            new Node({22, 24, 26, 28, 30}),
            new Node({31, 33, 35, 37, 39}),
            new Node({32, 34, 36, 38, 40}),
            new Node({41, 43, 45, 47, 49}),
            new Node({42, 44, 46, 48, 50}),
        };
        merged = mergeSortedLists(heads);
        for (int32_t i = 1; i <= 50; i++)
        {
            ASSERT_NE(merged, nullptr);
            EXPECT_EQ(merged->value, i);
            merged = merged->next;
        }
        EXPECT_EQ(merged, nullptr);
    }
}

namespace ex10
{
    uint32_t generateRandomData(const std::string &filePath, uint32_t n)
    {
        std::mt19937 generator(std::random_device{}());

        std::vector<uint32_t> data(n);
        std::iota(data.begin(), data.end(), 1);
        std::shuffle(data.begin(), data.end(), generator);

        uint32_t randomElement = data[std::uniform_int_distribution<uint32_t>(0, n - 1)(generator)];
        data.erase(std::remove(data.begin(), data.end(), randomElement));

        std::ofstream file(filePath, std::ios::trunc);
        for (auto element : data)
            file << element << std::endl;

        return randomElement;
    }

    uint32_t findMissingNumber(const std::string &filePath)
    {
        std::ifstream file(filePath);
        uint32_t element;
        uint32_t sum = 0;
        uint32_t n = 0;
        while (file >> element)
        {
            sum += element;
            n++;
        }

        return (n + 1) * (n + 2) / 2 - sum;
    }

    TEST(List4_Exercise10, findMissingNumber)
    {
        EXPECT_EQ(findMissingNumber("/tmp/random10.txt"), generateRandomData("/tmp/random10.txt", 10));
        EXPECT_EQ(findMissingNumber("/tmp/random1000.txt"), generateRandomData("/tmp/random1000.txt", 1000));
        EXPECT_EQ(findMissingNumber("/tmp/random1000000.txt"), generateRandomData("/tmp/random1000000.txt", 1000000));
    }
}