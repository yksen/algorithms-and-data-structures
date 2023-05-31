#include <gtest/gtest.h>

namespace ref1
{
    class UnionFind3
    {
    public:
        int n;
        int *parent;
        int *rank;

        UnionFind3(int n) : n(n), parent(new int[n]), rank(new int[n])
        {
            for (int i = 0; i < n; i++)
            {
                parent[i] = i;
                rank[i] = 0;
            }
        }

        ~UnionFind3()
        {
            delete[] parent;
            delete[] rank;
        }

        int Find(int I)
        {
            if (parent[I] != I)
                parent[I] = Find(parent[I]); // kompresja ścieżki

            return parent[I];
        }

        void Union(int I, int J)
        {
            I = Find(I);
            J = Find(J);
            if (rank[I] > rank[J]) // podpinamy niższe drzewko pod wyższe
                parent[J] = I;
            else
            {
                parent[I] = J;          // wysokość J większa lub równa wysokości I
                if (rank[I] == rank[J]) // tylko jak równe wysokości I i J
                    rank[J]++;          // to wysokość J się zwiększa
            }
        }
    };
}

namespace ex1
{
    typedef std::vector<std::vector<bool>> Grid;
    typedef std::pair<int32_t, int32_t> Point;

    void printGrid(bool *t, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                std::cout << t[i * n + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    bool doesPathExist(Grid t, Point start, Point end)
    {
        const size_t n = t.size();
        ref1::UnionFind3 uf(n * n);
        const int32_t dx[] = {-1, 1, 0, 0};
        const int32_t dy[] = {0, 0, -1, 1};

        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j)
                for (size_t k = 0; k < 4; ++k)
                {
                    size_t x = i + dx[k];
                    size_t y = j + dy[k];
                    if (x < n && y < n && t[i][j] && t[x][y])
                        uf.Union(i * n + j, x * n + y);
                }

        return uf.Find(start.first * n + start.second) == uf.Find(end.first * n + end.second);
    }

    TEST(List10_Exercise1, FirstToLastCell)
    {
        Grid t = {
            {1, 0, 1},
            {1, 1, 1},
            {0, 0, 1},
        };
        EXPECT_TRUE(doesPathExist(t, {0, 0}, {t.size() - 1, t.size() - 1}));
        EXPECT_FALSE(doesPathExist(t, {0, 0}, {t.size() - 1, t.size() - 2}));

        t = {
            {1, 0, 0},
            {1, 0, 1},
            {0, 0, 1},
        };
        EXPECT_FALSE(doesPathExist(t, {0, 0}, {t.size() - 1, t.size() - 1}));
    }

    TEST(List10_Exercise1, FirstToLastRow)
    {
        Grid t = {
            {1, 0, 1},
            {1, 1, 1},
            {0, 1, 0},
        };
        EXPECT_TRUE(doesPathExist(t, {0, 0}, {t.size() - 1, t.size() - 2}));
        EXPECT_TRUE(doesPathExist(t, {0, 2}, {t.size() - 1, t.size() - 2}));
        EXPECT_FALSE(doesPathExist(t, {0, 1}, {t.size() - 1, t.size() - 2}));
    }
}