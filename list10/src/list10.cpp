#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <queue>
#include <stack>

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

namespace ref4
{
    using namespace std;

    const double infinity = numeric_limits<double>::max();

    struct Edge
    {
        int target;    // vertex pointed by the edge
        double length; // length of the edge

        Edge(int v0, double d0) : target(v0), length(d0) {}
    };

    class Graph
    {
    public:
        vector<vector<Edge>> outgoingEdges; // outgoingEdges[A] - vector of edges outgoing from vertex A

        int nVertices() { return outgoingEdges.size(); } // number of vertices in the graph

        Graph(int n) : outgoingEdges(n) {}

        void addEdge(int A, int B, double distance = 1)
        {
            outgoingEdges[A].push_back(Edge(B, distance));
        }

        void addUndirectedEdge(int A, int B, double distance = 1)
        {
            outgoingEdges[A].push_back(Edge(B, distance));
            outgoingEdges[B].push_back(Edge(A, distance));
        }

        void show(bool with_weights = true)
        {
            int vertex = 0;
            for (auto outgoing : outgoingEdges)
            {
                cout << vertex++ << ":";
                for (auto edge : outgoing)
                {
                    cout << " " << edge.target;
                    if (with_weights)
                        cout << "(" << edge.length << ")";
                }
                cout << endl;
            }
        }

        void printAdjacencyMatrix()
        {
            std::cout << "  ";
            for (int A = 0; A < nVertices(); ++A)
                std::cout << A + 1 << " ";
            for (int A = 0; A < nVertices(); ++A)
            {
                std::cout << std::endl
                          << A + 1 << " ";
                for (int B = 0; B < nVertices(); ++B)
                {
                    bool found = false;
                    for (auto edge : outgoingEdges[A])
                    {
                        if (edge.target == B)
                        {
                            found = true;
                            std::cout << edge.length << " ";
                        }
                    }
                    if (!found)
                        std::cout << "0 ";
                }
            }
            std::cout << std::endl;
        }
    };

    class DFS
    {
        Graph &G;
        vector<bool> visited;

    public:
        DFS(Graph &G0) : G(G0), visited(G.nVertices(), false)
        {
        }

        void visit(int A)
        {
            cout << " " << A; // print out vertex number (visit the vertex)
            visited[A] = true;

            for (Edge AB : G.outgoingEdges[A])
            {
                int B = AB.target;
                if (!visited[B])
                    visit(B);
            }
        }

        int visitAll() // needed if the graph is not connected
        {
            int ile = 0;
            for (int A = 0; A < G.nVertices(); A++)
                if (!visited[A])
                {
                    visit(A);
                    ile++;
                }
            return ile;
        }

        void reset()
        {
            visited.assign(visited.size(), false);
        }
    };

    class BFS
    {
        Graph &G;
        vector<bool> inQueue; // remember if the vertex has been put to Queue

    public:
        BFS(Graph &G0) : G(G0),
                         inQueue(G.nVertices(), false)
        {
        }

        BFS &visit(int A)
        {
            queue<int> Q;
            Q.push(A);         // add starting vertex to Queue
            inQueue[A] = true; // and mark it (as pushed to Queue)

            while (Q.size() > 0) // white Q not empty
            {
                A = Q.front();
                Q.pop(); // take first vertex from Queue

                cout << " " << A; // print it out

                for (Edge AB : G.outgoingEdges[A]) // for all outgoing edges
                {
                    int B = AB.target; // B is neighbour of A

                    if (!inQueue[B]) // if B was not in Queue yet
                    {
                        Q.push(B);         // push it to Queue
                        inQueue[B] = true; // and mark as pushed
                    }
                }
            }
            cout << endl;
            return *this;
        }

        void visitAll() // needed if graph is not connected
        {
            for (int V = 0; V < G.nVertices(); V++)
                if (!inQueue[V])
                    visit(V);
        }

        void reset()
        {
            inQueue.assign(G.nVertices(), false); // unmark all the vertices
        }
    };

    class myPQ
    {
        vector<double> &distance;
        vector<int> node;
        vector<int> position;

    public:
        myPQ(vector<double> &dist0) : distance(dist0), node(dist0.size()), position(dist0.size())
        {
            for (size_t i = 0; i < distance.size(); i++)
            {
                distance[i] = infinity;
                node[i] = position[i] = i;
            }
        }

        void upwards(int n)
        {
            int i;
            while (n > 0 && distance[node[i = (n - 1) / 2]] > distance[node[n]])
            {
                swap(node[i], node[n]);
                position[node[i]] = i;
                position[node[n]] = n;
            }
        }

        void decrease_key(int i, double val)
        {
            distance[i] = val;
            upwards(position[i]);
        }

        int getmin()
        {
            int res = node[0];
            int i = 0;
            int k;
            int n = node.size() - 1;
            node[0] = node[n];
            node.pop_back();
            while (
                ((k = 2 * i + 2) < n && distance[node[i]] > distance[node[k]] && distance[node[k - 1]] > distance[node[k]]) ||
                (--k < n && distance[node[i]] > distance[node[k]]))
            {
                swap(node[i], node[k]);
                position[node[i]] = i;
                position[node[k]] = k;
            }
            return res;
        }

        int size()
        {
            return node.size();
        }
    };

    class Dijkstra
    {
        Graph &G;                // graph on which the algorithm runs on
        vector<double> distance; // distance[A] - distance from source vertex to A
        vector<int> previous;    // previous[A] - predecessor of A on the shortest path from source vertex to A

    public:
        Dijkstra(Graph &G0) : G(G0),
                              distance(G.nVertices(), infinity),
                              previous(G.nVertices(), 0)
        {
        }

        Dijkstra &findShortestPathsFrom(int A)
        {
            myPQ Q(distance);     // create PQ with all distances = infinity
            previous[A] = A;      // set A == beginning of the paths
            Q.decrease_key(A, 0); // decrease distance from A from A to zero

            while (Q.size() > 0)
            {
                A = Q.getmin();                    // A = take from Q the node closest to source (from all nodes remaining in Q)
                for (Edge AB : G.outgoingEdges[A]) // for all edges going out from A
                {
                    int B = AB.target;                         // B - other end of the edge
                    if (distance[A] + AB.length < distance[B]) // if the path through A is shorter than known so far
                    {
                        previous[B] = A;                            // set A as the predecessor of B
                        Q.decrease_key(B, distance[A] + AB.length); // decrease the distance to B
                    }
                }
            }

            return *this;
        }

        void showPrevious()
        {
            for (int i = 0; i < G.nVertices(); i++)
                cout << i << "(" << distance[i] << ")->" << previous[i] << " " << endl;
            cout << endl;
        }

        void reset()
        {
            for (int i = 0; i < G.nVertices(); i++)
            {
                distance[i] = infinity;
                previous[i] = 0;
            }
        }
    };

    class MST_Prim
    {
        Graph &G;
        vector<double> distance; // distance from the growing tree
        vector<int> previous;    // edge of the resulting tree

    public:
        MST_Prim(Graph &G0) : G(G0),
                              distance(G.nVertices(), infinity),
                              previous(G.nVertices(), 0)
        {
        }

        MST_Prim &MST(int A)
        {
            myPQ Q(distance);
            previous[A] = A;
            Q.decrease_key(A, 0);

            while (Q.size() > 0)
            {
                A = Q.getmin();                    // get the vertex A closest to the growing tree
                distance[A] = 0;                   // add it to MST (join with predecessor)
                for (Edge AB : G.outgoingEdges[A]) // update distances to its neighbours
                {
                    int B = AB.target;
                    if (AB.length < distance[B]) // if edge length closer than known before distance
                    {
                        previous[B] = A;              // update the predecessor
                        Q.decrease_key(B, AB.length); // and distance from the MST
                    }
                }
            }
            return *this;
        }

        void showPrevious()
        {
            for (int i = 0; i < G.nVertices(); i++)
                cout << i << "(" << distance[i] << ")->" << previous[i] << " ";
            cout << endl;
        }

        void reset()
        {
            for (int i = 0; i < G.nVertices(); i++)
            {
                distance[i] = infinity;
                previous[i] = 0;
            }
        }
    };

    class UnionFind
    {
    public:
        vector<int> parent;
        vector<int> rank;

        UnionFind(int n) : parent(n), rank(n, 0)
        {
            for (int i = 0; i < n; i++)
                parent[i] = i;
        }

        int Find(int i)
        {
            if (parent[i] != i)
                parent[i] = Find(parent[i]);
            return parent[i];
        }

        int Union(int i, int j)
        {
            int a = Find(i);
            int b = Find(j);
            if (a == b)
                return 0;
            if (rank[a] > rank[b])
                parent[b] = a;
            else
            {
                parent[a] = b;
                if (rank[a] == rank[b])
                    rank[b]++;
            }
            return 1;
        }
    };

    struct edge
    {
        int a;
        int b;
        double d;

        edge(int a0, int b0, double d0) : a(a0), b(b0), d(d0) {}

        bool operator<(edge e2)
        {
            return d < e2.d;
        }
    };

    Graph MST_Kruskal(Graph &G)
    {
        Graph tree(G.nVertices());

        // zrób tablicę wszystkich krawędzi grafu
        vector<edge> edges;

        for (int A = 0; A < G.nVertices(); A++)
            for (Edge AB : G.outgoingEdges[A])
                edges.push_back(edge(A, AB.target, AB.length));

        // sortuj krawędzie wg długości
        // sort(edges.begin(),edges.end(),[](edge e1, edge e2){return e1.d<e2.d;});
        sort(edges.begin(), edges.end());

        UnionFind uf(G.nVertices());
        // przetwarzaj krawędzie od najkrótszej do najdłuższej
        // akceptuj krawędź (a,b) jeśli nie było jeszcze połączenia między a i b
        for (edge edge : edges)
            if (uf.Union(edge.a, edge.b)) // prawda jeśli nie było połączenia
                tree.addUndirectedEdge(edge.a, edge.b, edge.d);
        return tree;
    }
}

typedef std::vector<std::vector<bool>> Grid;
typedef std::pair<int32_t, int32_t> Point;
const int32_t dx[] = {1, 0, 0, -1};
const int32_t dy[] = {0, 1, -1, 0};

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
        ref4::UnionFind uf(n * n);

        int32_t islandsCount = 0;

        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j)
            {
                if (t[i][j])
                    ++islandsCount;
                for (size_t k = 0; k < 2; ++k)
                {
                    size_t x = i + dx[k];
                    size_t y = j + dy[k];
                    if (x < n && y < n && t[i][j] && t[x][y])
                        islandsCount -= uf.Union(i * n + j, x * n + y);
                }
            }

        return islandsCount;
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

namespace ex4
{
    using namespace ref4;

    Graph constructFromFilestream(std::ifstream &file)
    {
        size_t n, e;
        file >> n >> e;
        Graph G(n);
        for (size_t i = 0; i < e; ++i)
        {
            size_t a, b;
            double d;
            file >> a >> b >> d;
            G.addEdge(a - 1, b - 1, d);
        }
        return G;
    }

    TEST(List10_Exercise4, Placeholder)
    {
        std::ifstream file("/mnt/c/Users/Kamil/Desktop/algorithms-and-data-structures/list10/src/graf");
        if (!file.is_open())
            return;

        Graph g = constructFromFilestream(file);
        g.printAdjacencyMatrix();
        std::cout << std::endl;

        Graph mst = MST_Kruskal(g);
        mst.printAdjacencyMatrix();
        std::cout << std::endl;

        Dijkstra d(g);
        d.findShortestPathsFrom(0).showPrevious();
        std::cout << std::endl;
    }
}

namespace ex5
{
    typedef std::vector<std::vector<uint32_t>> Chessboard;

    int32_t dx[] = {1, 1, -1, -1, 2, 2, -2, -2};
    int32_t dy[] = {2, -2, 2, -2, 1, -1, 1, -1};

    bool found = false;

    void depthFirstSearch(Chessboard &visited, Point p, uint32_t step)
    {
        if (found)
            return;

        visited[p.first][p.second] = step;

        const size_t n = visited.size();
        size_t newSteps = 0;

        for (size_t i = 0; i < 8; ++i)
        {
            size_t x = p.first + dx[i];
            size_t y = p.second + dy[i];
            if (x < n && y < n && !visited[x][y])
            {
                ++newSteps;
                depthFirstSearch(visited, {x, y}, step + 1);
            }
        }

        if (newSteps == 0)
            visited[p.first][p.second] = step;

        if (newSteps == 0 && step == n * n)
        {
            found = true;
            for (size_t i = 0; i < n; ++i)
            {
                for (size_t j = 0; j < n; ++j)
                    std::cout << std::setw(3) << visited[i][j];
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        visited[p.first][p.second] = 0;
    }

    void traverseChessboard(size_t n)
    {
        found = false;
        Chessboard visited(n, std::vector<uint32_t>(n, 0));
        depthFirstSearch(visited, {0, 0}, 1);
    }

    TEST(List10_Exercise5, traverseChessboard)
    {
        for (size_t i = 5; i < 8; ++i)
            traverseChessboard(i);
    }
}