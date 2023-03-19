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
            int32_t j = i - 1;
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
    }
}

