#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using std::vector, std::stack, std::pair, std::cin, std::cout, std::endl, std::fill;

void dfs(int v, const vector<vector<int>> &graph, vector<bool> &visited, stack<int> &finishStack)
{
    visited[v] = true;
    for (int neighbor : graph[v])
    {
        if (!visited[neighbor])
        {
            dfs(neighbor, graph, visited, finishStack);
        }
    }
    finishStack.push(v);
}

void reverseDfs(int v, const vector<vector<int>> &reverseGraph, vector<bool> &visited, vector<int> &component)
{
    visited[v] = true;
    component.push_back(v);
    for (int neighbor : reverseGraph[v])
    {
        if (!visited[neighbor])
        {
            reverseDfs(neighbor, reverseGraph, visited, component);
        }
    }
}

vector<vector<int>> kosaraju(int n, const vector<pair<int, int>> &edges)
{
    vector<vector<int>> graph(n + 1), reverseGraph(n + 1);
    for (const auto &edge : edges)
    {
        graph[edge.first].push_back(edge.second);
        reverseGraph[edge.second].push_back(edge.first);
    }

    vector<bool> visited(n + 1, false);
    stack<int> finishStack;

    for (int i = 1; i <= n; ++i)
    {
        if (!visited[i])
        {
            dfs(i, graph, visited, finishStack);
        }
    }

    fill(visited.begin(), visited.end(), false);
    vector<vector<int>> scc;

    while (!finishStack.empty())
    {
        int v = finishStack.top();
        finishStack.pop();
        if (!visited[v])
        {
            vector<int> component;
            reverseDfs(v, reverseGraph, visited, component);
            scc.push_back(component);
        }
    }

    return scc;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);

    for (int i = 0; i < m; ++i)
    {
        cin >> edges[i].second >> edges[i].first;
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
