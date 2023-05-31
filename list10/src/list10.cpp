#include <gtest/gtest.h>
#include <stack>
#include <unordered_set>

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

typedef std::vector<std::vector<bool>> Grid;
typedef std::pair<int32_t, int32_t> Point;
const int32_t dx[] = {-1, 1, 0, 0};
const int32_t dy[] = {0, 0, -1, 1};

namespace ex1
{
    bool doesPathExist(Grid t, Point start, Point end)
    {
        const size_t n = t.size();
        ref1::UnionFind3 uf(n * n);

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

namespace ex2
{
    size_t getIslandsCount(Grid t)
    {
        const size_t n = t.size();
        ref1::UnionFind3 uf(n * n);

        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j)
                for (size_t k = 0; k < 4; ++k)
                {
                    size_t x = i + dx[k];
                    size_t y = j + dy[k];
                    if (x < n && y < n && t[i][j] && t[x][y])
                        uf.Union(i * n + j, x * n + y);
                }

        std::unordered_set<int32_t> islands;
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j)
                if (t[i][j])
                    islands.insert(uf.Find(i * n + j));

        return islands.size();
    }

    size_t getIslandsCountDFS(Grid t)
    {
        const size_t n = t.size();
        Grid visited(n, std::vector<bool>(n, false));

        size_t islands = 0;
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j)
                if (!visited[i][j] && t[i][j])
                {
                    ++islands;
                    std::stack<Point> toVisit;
                    toVisit.push({i, j});
                    while (!toVisit.empty())
                    {
                        Point p = toVisit.top();
                        toVisit.pop();
                        if (visited[p.first][p.second])
                            continue;
                        visited[p.first][p.second] = true;
                        for (size_t k = 0; k < 4; ++k)
                        {
                            size_t x = p.first + dx[k];
                            size_t y = p.second + dy[k];
                            if (x < n && y < n && t[x][y])
                                toVisit.push({x, y});
                        }
                    }
                }

        return islands;
    }

    TEST(List10_Exercise2, IslandsCount)
    {
        Grid t = {
            {1, 1, 0, 1, 0},
            {1, 1, 0, 1, 0},
            {0, 0, 1, 0, 0},
            {1, 1, 0, 1, 1},
            {0, 0, 0, 1, 1},
        };
        EXPECT_EQ(getIslandsCount(t), 5);
        EXPECT_EQ(getIslandsCountDFS(t), 5);

        t = {
            {1, 0, 1},
            {0, 1, 0},
            {1, 0, 1},
        };
        EXPECT_EQ(getIslandsCount(t), 5);
        EXPECT_EQ(getIslandsCountDFS(t), 5);

        t = {
            {1, 1, 1},
            {1, 1, 0},
            {1, 0, 1},
        };
        EXPECT_EQ(getIslandsCount(t), 2);
        EXPECT_EQ(getIslandsCountDFS(t), 2);
    }
}