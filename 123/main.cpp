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
/**

#include "kosaraju.hpp"
int main()
{
    int n = 60000;
    vector<pair<int, int>> edges;

    for (int i = 1; i < n; ++i)
    {
        edges.emplace_back(i, i + 1);
        edges.emplace_back(i + 1, i);
    }
    edges.emplace_back(n, 1);

    vector<vector<int>> scc_list = kosaraju_list(n, edges);
    vector<vector<int>> scc_deque = kosaraju_deque(n, edges);

    cout << "Finished processing with list and deque." << endl;
    return 0;
}
*
 */