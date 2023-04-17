#include <gtest/gtest.h>

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
    }
}