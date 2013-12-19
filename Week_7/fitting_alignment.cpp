#include <string>
#include <tuple>
#include "graph.h"
#include <algorithm>
#include <thread>
#include <iostream>

using namespace std;

string to_key(const size_t i, const size_t j);

static void node_initializing(graph& g, const size_t n, const size_t m)
{
  for (size_t i = 0; i < n + 1; i++)
  {
    for (size_t j = 0; j < m + 1; j++)
    {
      g.nodes[to_key(i + 1, j)];
      g.nodes[to_key(i, j)];
    }
  }
}

// Deletions graph
static void gen_upper_graph(graph& g_upper, graph& g_middle, const size_t n, const size_t m)
{
  const int sigma = 1;
  const int jump_cost = 0;

  for (size_t i = 0; i < n + 1; i++)
  {
    for (size_t j = 0; j < m + 1; j++)
    {
      // Middle to upper input edges
      for (size_t k = 0; k < i; ++k)
        g_upper.nodes[to_key(i, j)].input_edges.push_back(edge(jump_cost, g_middle.nodes.find(to_key(k, j)), jump));

      // Upper to middle output edges
      for (size_t k = i + 1; k < n + 1; ++k)
        g_upper.nodes[to_key(i, j)].output_edges.push_back(edge(jump_cost, g_middle.nodes.find(to_key(k, j)), jump));

      if (i < n && j < m)
      {
        g_upper.nodes[to_key(i, j)].output_edges.push_back(edge(-sigma, g_upper.nodes.find(to_key(i + 1, j)), deletion));
        g_upper.nodes[to_key(i + 1, j)].input_edges.push_back(edge(-sigma, g_upper.nodes.find(to_key(i, j)), deletion));
      }

      // Last column
      if (i == n)
      {
        g_upper.nodes[to_key(i, j)].input_edges.push_back(edge(-sigma, g_upper.nodes.find(to_key(i - 1, j)), deletion));
      }
    }
  }
}

// Insertion graph
static void gen_bottom_graph(graph& g_bottom, graph& g_middle, const size_t n, const size_t m)
{
  const int sigma = 1;
  const int jump_cost = 0;

  for (size_t i = 0; i < n + 1; i++)
  {
    for (size_t j = 0; j < m + 1; j++)
    {
      // Middle to bottom input edges
      for (size_t k = 0; k < j; ++k)
        g_bottom.nodes[to_key(i, j)].input_edges.push_back(edge(jump_cost, g_middle.nodes.find(to_key(k, i)), jump));

      // Bottom to middle output edges
      for (size_t k = j + 1; k < n + 1; ++k)
        g_bottom.nodes[to_key(i, j)].output_edges.push_back(edge(jump_cost, g_middle.nodes.find(to_key(k, i)), jump));

      if (i < n && j < m)
      {
        g_bottom.nodes[to_key(i, j)].output_edges.push_back(edge(-sigma, g_bottom.nodes.find(to_key(i, j + 1)), insertion));
        g_bottom.nodes[to_key(i, j + 1)].input_edges.push_back(edge(-sigma, g_bottom.nodes.find(to_key(i, j)), insertion));
      }

      // Last row
      if (j == m)
      {
        g_bottom.nodes[to_key(i, j)].input_edges.push_back(edge(-sigma, g_bottom.nodes.find(to_key(i, j - 1)), insertion));
      }
    }
  }
}

// Matches/Mismatches graph
static void gen_middle_graph(graph& g_middle, graph& g_upper, graph& g_bottom, const string& s1, const string& s2)
{
  const int sigma = 1;
  const int jump_cost = 0;

  for (size_t i = 0; i < s1.length() + 1; i++)
  {
    for (size_t j = 0; j < s2.length() + 1; j++)
    {
      // Upper to middle input edges
      for (size_t k = 0; k < i; ++k)
        g_middle.nodes[to_key(i, j)].input_edges.push_back(edge(jump_cost, g_upper.nodes.find(to_key(k, j)), jump));

      // Middle ot upper output edges
      for (size_t k = i + 1; k < s1.length() + 1; ++k)
        g_middle.nodes[to_key(i, j)].output_edges.push_back(edge(jump_cost, g_upper.nodes.find(to_key(k, j)), jump));

      // Bottom to middle input edges
      for (size_t k = 0; k < j; ++k)
        g_middle.nodes[to_key(i, j)].output_edges.push_back(edge(jump_cost, g_bottom.nodes.find(to_key(k, i)), jump));

      // Middle to upper output edges
      for (size_t k = j + 1; k < s2.length(); ++k)
        g_middle.nodes[to_key(i, j)].output_edges.push_back(edge(jump_cost, g_bottom.nodes.find(to_key(k, i)), jump));

      if (i < s1.length() && j < s2.length())
      {
        g_middle.nodes[to_key(i, j)].output_edges.push_back(edge(s1[i] == s2[j] ? sigma : -sigma, g_middle.nodes.find(to_key(i + 1, j + 1)), matches));
        g_middle.nodes[to_key(i + 1, j + 1)].input_edges.push_back(edge(s1[i] == s2[j] ? sigma : -sigma, g_middle.nodes.find(to_key(i, j)), matches));
      }

      // Last column
      if (i == s1.length())
      {
        // Except first and last node
        if (j != 0 && j != s1.length())
          g_middle.nodes[to_key(i, j)].input_edges.push_back(edge(s1[i] == s2[j] ? sigma : -sigma, g_middle.nodes.find(to_key(i - 1, j - 1)), matches));
      }

      // Last row
      if (j == s2.length())
      {
        // Except first node
        if (i != 0)
          g_middle.nodes[to_key(i, j)].input_edges.push_back(edge(s1[i] == s2[j] ? sigma : -sigma, g_middle.nodes.find(to_key(i - 1, j - 1)), matches));
      }
    }
  }
}

static map<string, int> memo;

static int traverse_path(graph& g, map<string, node>::iterator n)
{
  int ret = 0;
  vector<pair<vector<edge>::reverse_iterator, int>> weights;
  for (auto input_edge = n->second.input_edges.rbegin(); input_edge != n->second.input_edges.rend(); ++input_edge)
  {
    auto key = input_edge->to->first;
    if (memo.find(key) == memo.end())
      memo[key] = traverse_path(g, input_edge->to);

    weights.push_back({ input_edge, input_edge->weight + memo[key] });
  }

  if (!weights.empty())
  {
    auto it = max_element(weights.begin(), weights.end(),
      [](const pair<vector<edge>::reverse_iterator, int>& lhs, const pair<vector<edge>::reverse_iterator, int>& rhs)
    {
      return lhs.second < rhs.second;
    });

    ret = it->second;
    it->first->visited = true;
  }

  n->second.value = ret;
  return ret;
}

static pair<size_t, size_t> key_to_idx(const string& key)
{
  size_t pos = key.find(",");
  return make_pair(stoi(key.substr(0, pos)), stoi(key.substr(pos + 1)));
}

static pair<string, string> backtrace(const graph& g, map<string, node>::iterator n, const string& s1, const string& s2)
{
  auto it = n;
  string ret1, ret2;
  while (true)
  {
    for (const auto& e : it->second.input_edges)
    {
      if (e.visited == true)
      {
        auto idx = key_to_idx(e.to->first);
        switch (e.type)
        {
        case deletion:
        {
                       ret1 += s1[idx.first];
                       ret2 += '-';
                       break;
        }
        case matches:
        {
                      ret1 += s1[idx.first];
                      ret2 += s2[idx.second];
                      break;
        }
        case insertion:
        {
                        ret2 += s2[idx.second];
                        ret1 += '-';
                        break;
        }
        }

        it = e.to;
        break;
      }
    }

    if (it->second.input_edges.empty())
      break;
  }

  return make_pair(string(ret1.rbegin(), ret1.rend()), string(ret2.rbegin(), ret2.rend()));
}

tuple<int, string, string> fitting_alignment(const string& s1, const string& s2)
{
  graph g_upper, g_middle, g_bottom;

  {
    vector<thread> thread_pool;
    thread_pool.push_back(thread([&](){ node_initializing(g_upper, s1.length(), s2.length()); }));
    thread_pool.push_back(thread([&](){ node_initializing(g_middle, s1.length(), s2.length()); }));
    thread_pool.push_back(thread([&](){ node_initializing(g_bottom, s1.length(), s2.length()); }));

    for (auto& th : thread_pool) th.join();
  }

  {
    vector<thread> thread_pool;
    thread_pool.push_back(thread([&](){ gen_upper_graph(g_upper, g_middle, s1.length(), s2.length()); }));
    thread_pool.push_back(thread([&](){ gen_bottom_graph(g_bottom, g_middle, s1.length(), s2.length()); }));
    thread_pool.push_back(thread([&](){ gen_middle_graph(g_middle, g_upper, g_bottom, s1, s2); }));

    for (auto& th : thread_pool) th.join();
  }

  //auto score = traverse_path(g, g.nodes.find(to_key(s1.length(), s2.length())));
  //auto ret = backtrace(g, g.nodes.find(to_key(s1.length(), s2.length())), s1, s2);

  return make_tuple(0, "", "");
}