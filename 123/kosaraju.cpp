#include "kosaraju.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>

using namespace std;
void dfs_list(int v, const vector<list<int>> &graph, vector<bool> &visited, stack<int> &finishStack)
{
    visited[v] = true;
    for (int neighbor : graph[v])
    {
        if (!visited[neighbor])
        {
            dfs_list(neighbor, graph, visited, finishStack);
        }
    }
    finishStack.push(v);
}

void reverseDfs_list(int v, const vector<list<int>> &reverseGraph, vector<bool> &visited, vector<int> &component)
{
    visited[v] = true;
    component.push_back(v);
    for (int neighbor : reverseGraph[v])
    {
        if (!visited[neighbor])
        {
            reverseDfs_list(neighbor, reverseGraph, visited, component);
        }
    }
}

vector<vector<int>> kosaraju_list(int n, const vector<pair<int, int>> &edges)
{
    vector<list<int>> graph(n + 1), reverseGraph(n + 1);
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
            dfs_list(i, graph, visited, finishStack);
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
            reverseDfs_list(v, reverseGraph, visited, component);
            scc.push_back(component);
        }
    }

    return scc;
}

void dfs_deq(int v, const vector<deque<int>> &graph, vector<bool> &visited, stack<int> &finishStack)
{
    visited[v] = true;
    for (int neighbor : graph[v])
    {
        if (!visited[neighbor])
        {
            dfs_deq(neighbor, graph, visited, finishStack);
        }
    }
    finishStack.push(v);
}

void reverseDfs_deq(int v, const vector<deque<int>> &reverseGraph, vector<bool> &visited, vector<int> &component)
{
    visited[v] = true;
    component.push_back(v);
    for (int neighbor : reverseGraph[v])
    {
        if (!visited[neighbor])
        {
            reverseDfs_deq(neighbor, reverseGraph, visited, component);
        }
    }
}

vector<vector<int>> kosaraju_deque(int n, const vector<pair<int, int>> &edges)
{
    vector<deque<int>> graph(n + 1), reverseGraph(n + 1);
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
            dfs_deq(i, graph, visited, finishStack);
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
            reverseDfs_deq(v, reverseGraph, visited, component);
            scc.push_back(component);
        }
    }

    return scc;
}
