#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include "graph.h"

struct L3_fitting_graph
{
  graph g_upper, g_middle, g_bottom;
  void node_initializing(const size_t n, const size_t m)
  {
    auto func = [n, m](graph& g)
    {
      for (size_t i = 0; i < n + 1; i++)
      {
        for (size_t j = 0; j < m + 1; j++)
        {
          g.nodes[to_key(i + 1, j)];
          g.nodes[to_key(i, j)];
        }
      }
    };

    std::vector<std::thread> thread_pool;
    thread_pool.push_back(std::thread([&](){ func(g_upper); }));
    thread_pool.push_back(std::thread([&](){ func(g_middle); }));
    thread_pool.push_back(std::thread([&](){ func(g_bottom); }));

    for (auto& th : thread_pool) th.join();
  }

  void gen_graph(const std::string& s1, const std::string& s2)
  {
    std::vector<std::thread> thread_pool;

    // Deletions graph
    thread_pool.push_back(std::thread([&]()
    { 
      const size_t n = s1.length();
      const size_t m = s2.length();
      const int epsilon = -1;
      const int sigma = -1;
      const int jump_cost = 0;

      for (size_t i = 0; i < n + 1; i++)
      {
        for (size_t j = 0; j < m + 1; j++)
        {
          // Middle to upper input edge
          if (i != 0)
            g_upper.nodes[to_key(i, j)].input_edges.push_back(edge(sigma, g_middle.nodes.find(to_key(i - 1, j)), jump_to_middle));

          // Upper to middle output edge
          g_upper.nodes[to_key(i, j)].output_edges.push_back(edge(jump_cost, g_middle.nodes.find(to_key(i, j)), jump_to_middle));

          if (i < n && j < m)
          {
            g_upper.nodes[to_key(i, j)].output_edges.push_back(edge(epsilon, g_upper.nodes.find(to_key(i + 1, j)), deletion));
            g_upper.nodes[to_key(i + 1, j)].input_edges.push_back(edge(epsilon, g_upper.nodes.find(to_key(i, j)), deletion));
          }

          // Last column
          if (i == n)
          {
            g_upper.nodes[to_key(i, j)].input_edges.push_back(edge(epsilon, g_upper.nodes.find(to_key(i - 1, j)), deletion));
          }
        }
      }
    }));
    
    // Insertion graph
    thread_pool.push_back(std::thread([&]()
    { 
      const size_t n = s1.length();
      const size_t m = s2.length();
      const int sigma = -1;
      const int epsilon = -1;
      const int jump_cost = 0;

      for (size_t i = 0; i < n; i++)
      {
        for (size_t j = 0; j < m; j++)
        {
          // Middle to bottom input edge
          if (j != 0)
            g_bottom.nodes[to_key(i, j)].input_edges.push_back(edge(sigma, g_middle.nodes.find(to_key(i, j - 1)), jump_to_middle));

          // Bottom to middle output edge
          g_bottom.nodes[to_key(i, j)].output_edges.push_back(edge(jump_cost, g_middle.nodes.find(to_key(i, j)), jump_to_middle));

          if (i < n && j < m)
          {
            g_bottom.nodes[to_key(i, j)].output_edges.push_back(edge(epsilon, g_bottom.nodes.find(to_key(i, j + 1)), insertion));
            g_bottom.nodes[to_key(i, j + 1)].input_edges.push_back(edge(epsilon, g_bottom.nodes.find(to_key(i, j)), insertion));
          }

          // Last row
          if (j == m)
          {
            g_bottom.nodes[to_key(i, j)].input_edges.push_back(edge(epsilon, g_bottom.nodes.find(to_key(i, j - 1)), insertion));
          }
        }
      }
    }));

    // Matches/Mismatches graph
    thread_pool.push_back(std::thread([&]()
    { 
      const int sigma = -1;
      const int jump_cost = 0;

      for (size_t i = 0; i < s1.length() + 1; i++)
      {
        for (size_t j = 0; j < s2.length() + 1; j++)
        {
          // Upper to middle input edge
          if (i != 0)
            g_middle.nodes[to_key(i, j)].input_edges.push_back(edge(jump_cost, g_upper.nodes.find(to_key(i, j)), jump_to_upper));

          // Middle ot upper output edge
          if (i != s1.length())
            g_middle.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g_upper.nodes.find(to_key(i + 1, j)), jump_to_upper));

          // Bottom to middle input edge
          if (i != 0)
            g_middle.nodes[to_key(i, j)].input_edges.push_back(edge(jump_cost, g_bottom.nodes.find(to_key(i, j)), jump_to_bottom));

          // Middle to bottom output edge
          if (j != s2.length())
            g_middle.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g_bottom.nodes.find(to_key(i, j + 1)), jump_to_bottom));

          // Add edge from source to every node in 1st row
          if (j == 0 && i != 0)
          {
            g_middle.nodes[to_key(0, 0)].output_edges.push_back(edge(0, g_middle.nodes.find(to_key(i, j)), none));
            g_middle.nodes[to_key(i, j)].input_edges.push_back(edge(0, g_middle.nodes.find(to_key(0, 0)), none));
          }

          // Add edge from every node in last row to sink
          if (j == s2.length() && i != s1.length())
          {
            g_middle.nodes[to_key(s1.length(), s2.length())].input_edges.push_back(edge(0, g_middle.nodes.find(to_key(i, j)), none));
            g_middle.nodes[to_key(i, j)].output_edges.push_back(edge(0, g_middle.nodes.find(to_key(s1.length(), s2.length())), none));
          }

          if (i < s1.length() && j < s2.length())
          {
            g_middle.nodes[to_key(i, j)].output_edges.push_back(edge(s1[i] == s2[j] ? -sigma : sigma, g_middle.nodes.find(to_key(i + 1, j + 1)), matches));
            g_middle.nodes[to_key(i + 1, j + 1)].input_edges.push_back(edge(s1[i] == s2[j] ? -sigma : sigma, g_middle.nodes.find(to_key(i, j)), matches));
          }

          // Last column
          if (i == s1.length())
          {
            // Except first and last node
            if (j != 0 && j != s1.length())
              g_middle.nodes[to_key(i, j)].input_edges.push_back(edge(s1[i] == s2[j] ? -sigma : sigma, g_middle.nodes.find(to_key(i - 1, j - 1)), matches));
          }

          // Last row
          if (j == s2.length())
          {
            // Except first node
            if (i != 0)
              g_middle.nodes[to_key(i, j)].input_edges.push_back(edge(s1[i] == s2[j] ? -sigma : sigma, g_middle.nodes.find(to_key(i - 1, j - 1)), matches));
          }
        }
      }
    
    }));
    
    for (auto& th : thread_pool) th.join();
  }
};