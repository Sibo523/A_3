#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <deque>
#include <algorithm>
#include <list>
using std::vector, std::stack, std::pair, std::cin, std::cout, std::endl, std::fill, std::deque, std::list;

vector<vector<int>> kosaraju_list(int n, const vector<pair<int, int>> &edges);
vector<vector<int>> kosaraju_deque(int n, const vector<pair<int, int>> &edges);