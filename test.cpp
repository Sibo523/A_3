#include <doctest.h>
#include "kosaraju.cpp"
TEST_CASE("kosarajaru")
{
    vector<pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 1}, {3, 4}, {4, 5}, {5, 6}, {6, 4}, {7, 6}, {7, 8}, {8, 9}, {9, 7}};
    vector<vector<int>> scc = kosaraju(9, edges);
    CHECK(scc.size() == 3);
    CHECK(scc[0] == vector<int>({1, 3, 2}));
    CHECK(scc[1] == vector<int>({4, 6, 5}));
    CHECK(scc[2] == vector<int>({7, 9, 8}));
}