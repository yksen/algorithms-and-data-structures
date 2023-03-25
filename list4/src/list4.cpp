#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>
#include <random>

namespace ex10
{
    int64_t generateRandomData(const std::string &filePath, int64_t n)
    {
        std::mt19937 generator(std::random_device{}());

        std::vector<int64_t> data(n);
        std::iota(data.begin(), data.end(), 1);
        std::shuffle(data.begin(), data.end(), generator);

        int64_t randomElement = data[std::uniform_int_distribution<int64_t>(0, n - 1)(generator)];
        std::remove(data.begin(), data.end(), randomElement);

        std::ofstream file(filePath);
        for (auto &element : data)
            file << element << std::endl;
        return randomElement;
    }

    int64_t findMissingNumber(const std::string &filePath)
    {
        return 0;
    }

    TEST(List4_Exercise10, findMissingNumber)
    {
        const std::string filePath = "randomData.txt";
        EXPECT_EQ(generateRandomData(filePath, 10), findMissingNumber(filePath));
        EXPECT_EQ(generateRandomData(filePath, 1e3), findMissingNumber(filePath));
        EXPECT_EQ(generateRandomData(filePath, 1e6), findMissingNumber(filePath));
    }
}