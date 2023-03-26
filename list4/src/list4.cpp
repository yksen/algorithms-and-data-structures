#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>
#include <random>

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
        EXPECT_EQ(findMissingNumber("random10.txt"), generateRandomData("random10.txt", 10));
        EXPECT_EQ(findMissingNumber("random1000.txt"), generateRandomData("random1000.txt", 1000));
        EXPECT_EQ(findMissingNumber("random1000000.txt"), generateRandomData("random1000000.txt", 1000000));
    }
}