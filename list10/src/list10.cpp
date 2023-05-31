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
    bool doesPathExist(bool *t, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                std::cout << t[i * n + j] << " ";
            }
            std::cout << std::endl;
        }

        return true;
    }

    TEST(List10_Exercise1, FirstToLastCell)
    {
        const size_t n = 3;
        bool t[n][n] = {
            {1, 0, 1},
            {1, 1, 1},
            {0, 0, 1},
        };
        std::cout << doesPathExist((bool *)t, n) << std::endl;
    }

    TEST(List10_Exercise1, FirstToLastRow)
    {
    }
}