#include "kosaraju.hpp"

int main()
{
    int n, m;
    newGraph(n, m);
    vector<pair<int, int>> edges(m);

    for (int i = 0; i < m; ++i)
    {
        int j, k;
        cout << "Enter the edge (i to j): ";
        cin >> j >> k;
        Newedge(j, k, edges);
    }

    vector<vector<int>> scc = kosaraju(n, edges);

    for (const auto &component : scc)
    {
        for (int v : component)
        {
            cout << v << " ";
        }
        cout << endl;
    }

    return 0;
}
