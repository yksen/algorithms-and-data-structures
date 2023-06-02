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
    using reference1::Punkt, reference1::dist;
    Punkt A[] = {{0, 0}, {1, 0}, {2, 1}, {2, 2}, {1.5, 3}, {0.5, 3}, {0, 2.5}, {-1, 1}};

    double shortestLength(size_t a, size_t b)
    {
        const size_t n = sizeof(A) / sizeof(A[0]);
        double length[n][n] = {0};
        for (size_t i = 3; i <= b - a; ++i)
            for (size_t j = a; j <= b - i; ++j)
            {
                size_t k = j + i;
                for (size_t l = j + 1; l < k; ++l)
                {
                    double currentLength = length[j][l] + length[l][k];
                    if (j + 1 < l)
                        currentLength += dist(A[j], A[l]);
                    if (l + 1 < k)
                        currentLength += dist(A[l], A[k]);
                    if (currentLength < length[j][k] || length[j][k] == 0)
                        length[j][k] = currentLength;
                }
            }
        return length[a][b];
    }

    const int32_t w[] = {1, 100, 1, 100, 4, 5, 6, 7};

    uint32_t minimumCost(size_t a, size_t b)
    {
        const size_t n = sizeof(w) / sizeof(w[0]) - 1;
        uint32_t cost[n][n] = {0};
        for (size_t range = 1; range < n; ++range)
            for (size_t first = 0; first < n - range; ++first)
            {
                size_t last = first + range;
                for (size_t middle = first; middle < last; ++middle)
                {
                    uint32_t currentCost =
                        cost[first][middle] + cost[middle + 1][last] + w[first] * w[middle + 1] * w[last + 1];
                    if (currentCost < cost[first][last] || cost[first][last] == 0)
                        cost[first][last] = currentCost;
                }
            }
        return cost[a][b];
    }

    TEST(List9_Exercise2, shortestLength)
    {
        using namespace reference1;
        EXPECT_EQ(shortestLength(0, n - 1), BestLen(0, n - 1));
    }

    TEST(List9_Exercise2, minimumCost)
    {
        using namespace reference2;
        EXPECT_EQ(minimumCost(0, n - 1), KosztMin(0, n - 1));
    }
}

namespace ex6
{
    std::pair<std::vector<int32_t>, std::vector<int32_t>> cutRod(const std::vector<uint32_t> &prices, int32_t cost)
    {
        const size_t n = prices.size();
        std::vector<int32_t> maxRevenue(n, 0);
        std::vector<int32_t> shortestBit(n, 0);
        for (size_t i = 1; i < n; ++i)
        {
            int32_t currentMaxRevenue = std::numeric_limits<int32_t>::min();
            for (size_t j = 1; j <= i; ++j)
            {
                int32_t currentRevenue = prices[j] + maxRevenue[i - j] - cost;
                if (currentRevenue > currentMaxRevenue)
                {
                    currentMaxRevenue = currentRevenue;
                    shortestBit[i] = j;
                }
            }
            maxRevenue[i] = currentMaxRevenue;
        }
        return {maxRevenue, shortestBit};
    }

    void printCutRodSolution(const std::vector<uint32_t> &prices, int32_t cost)
    {
        auto [maxRevenue, shortestBit] = cutRod(prices, cost);
        size_t n = prices.size() - 1;
        std::cout << "Max revenue: " << maxRevenue[n] << ", rod lengths: ";
        while (n > 0)
        {
            std::cout << shortestBit[n] << " ";
            n -= shortestBit[n];
        }
        std::cout << std::endl;
    }

    TEST(List9_Exercise6, cutRod)
    {
        std::vector<uint32_t> prices = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
        printCutRodSolution(prices, 0);
        auto [maxRevenue, shortestBit] = cutRod(prices, 0);
        EXPECT_EQ(maxRevenue, std::vector<int32_t>({0, 1, 5, 8, 10, 13, 17, 18, 22, 25, 30}));
        EXPECT_EQ(shortestBit, std::vector<int32_t>({0, 1, 2, 3, 2, 2, 6, 1, 2, 3, 10}));

        printCutRodSolution(prices, 1);
        printCutRodSolution(prices, 2);
        printCutRodSolution(prices, 3);
    }
}