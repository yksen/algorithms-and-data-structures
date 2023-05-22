#include <cmath>
#include <gtest/gtest.h>

namespace reference1
{
    struct Punkt
    {
        double x;
        double y;
    };

    double dist(Punkt A, Punkt B)
    {
        double dx = A.x - B.x;
        double dy = A.y - B.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    Punkt A[] = {{0, 0}, {1, 0}, {2, 1}, {2, 2}, {1.5, 3}, {0.5, 3}, {0, 2.5}, {-1, 1}};

    const int n = sizeof(A) / sizeof(A[0]);

    int W[n][n] = {0};

    double BestLen(int a, int b)
    {
        static double L[n][n] = {0};

        if (L[a][b] > 0)
            return L[a][b];

        if (b < a + 3)
            return 0;

        double m = 0;

        for (int c = a + 1; c < b; c++)
        {

            double koszt = BestLen(a, c) + BestLen(c, b);
            if (a + 1 < c)
                koszt += dist(A[a], A[c]);
            if (c + 1 < b)
                koszt += dist(A[c], A[b]);

            if (koszt < m || m == 0)
            {
                m = koszt;
                W[a][b] = c;
            }
        }
        return L[a][b] = m;
    }
}

namespace reference2
{
    const int n = 7;
    int w[n + 1] = {1, 100, 1, 100, 4, 5, 6, 7};
    int Last[n][n] = {0};

    double KosztMin(int a, int b)
    {
        static double K[n][n] = {0};
        if (K[a][b] > 0)
            return K[a][b];
        if (b == a)
            return 0;

        double m = 0;
        for (int l = a; l < b; l++)
        {
            double koszt = KosztMin(a, l) + KosztMin(l + 1, b) + w[a] * w[l + 1] * w[b + 1];
            if (koszt < m || m == 0)
            {
                m = koszt;
                Last[a][b] = l;
            }
        }
        return K[a][b] = m;
    }
}

namespace ex2
{
    const size_t n = 7;
    const int32_t w[n + 1] = {1, 100, 1, 100, 4, 5, 6, 7};

    uint32_t minimumCost(size_t a, size_t b)
    {
        uint32_t cost[n][n] = {0};
        for (size_t i = 1; i < n; ++i)
            for (size_t j = 0; j < n - i; ++j)
            {
                size_t k = j + i;
                cost[j][k] = std::numeric_limits<uint32_t>::max();
                for (size_t l = j; l < k; ++l)
                {
                    uint32_t c = cost[j][l] + cost[l + 1][k] + w[j] * w[l + 1] * w[k + 1];
                    if (c < cost[j][k])
                        cost[j][k] = c;
                }
            }
        return cost[a][b];
    }

    TEST(List9_Exercise2, minimumCost)
    {
        using namespace reference2;
        EXPECT_EQ(minimumCost(0, n - 1), KosztMin(0, n - 1));
    }
}