#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <fstream>
#include "graph.h"
#include <iostream>

using namespace std;

vector<string> split_string(const string& s);
string to_key(const size_t i, const size_t j);

static void gen_matrix(map<string, int>& m)
{
  string matrix_file = "pam.dat";
  vector<string> alphabeth = { "A", "C", "D", "E", "F", "G", "H", "I", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "V", "W", "Y" };
  ifstream f(matrix_file);
  string temp;

  getline(f, temp); // Ignore first string
  while (getline(f, temp))
  {
    auto buff(split_string(temp));
    auto prefix = buff[0];
    size_t i = 1;
    for each (const auto& var in alphabeth)
    {
      m[prefix + var] = stoi(buff[i++]);
    }
  }
}

static void gen_graph(graph& g, map<string, int>& m, const string& s1, const string& s2)
{
  const int sigma = -5;

  for (size_t i = 0; i < s1.length() + 1; i++)
  {
    for (size_t j = 0; j < s2.length() + 1; j++)
    {
      // Add edge from source to every node, and from every node to sink
      if ( !(i == 0 && j == 0) && !(i == s1.length() && j == s2.length()))
      {
        g.nodes[to_key(0, 0)].output_edges.push_back(edge(0, g.nodes.find(to_key(i, j))));
        g.nodes[to_key(i, j)].input_edges.push_back(edge(0, g.nodes.find(to_key(0, 0))));

        g.nodes[to_key(s1.length(), s2.length())].input_edges.push_back(edge(0, g.nodes.find(to_key(i, j))));
        g.nodes[to_key(i, j)].output_edges.push_back(edge(0, g.nodes.find(to_key(s1.length(), s2.length()))));
      }

      if (i < s1.length() && j < s2.length())
      {
        g.nodes[to_key(i + 1, j + 1)];
        g.nodes[to_key(i, j + 1)];
        g.nodes[to_key(i + 1, j)];
        g.nodes[to_key(i, j)];

        string key{ s1[i], s2[j] };

        // Deletion
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j + 1)), deletion, '-'));
        // Insertion
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i + 1, j)), insertion));
        // Matches
        g.nodes[to_key(i, j)].output_edges.push_back(edge(m[key], g.nodes.find(to_key(i + 1, j + 1)), matches, s1[i] == s2[j] ? s1[i] : s2[j]));

        // Deletion
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), deletion, '-'));
        // Insertion
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), insertion));
        // Matches
        g.nodes[to_key(i + 1, j + 1)].input_edges.push_back(edge(m[key], g.nodes.find(to_key(i, j)), matches, s1[i]));

      }

      if (i == s1.length() && j != s2.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j + 1)), deletion));
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), insertion));
      }

      if (j == s2.length() && i != s1.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i + 1, j)), insertion));
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), deletion));
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

static int comp_score(const string& s1, const string& s2, map<string, int>& m)
{
  size_t idx = 0;
  int score = 0;
  while (idx < s1.length())
  {
    if (s1[idx] == '-' || s2[idx] == '-')
      score += -5;
    else
    {
      string key{ s1[idx], s2[idx] };
      score += m[key];
    }
      
    idx++;
  }

  return score;
}

tuple<int, string, string> local_alignment(string& s1, string& s2)
{
  if (s1.length() < s2.length())
    swap(s1, s2);

  map<string, int> PAM_matrix;
  gen_matrix(PAM_matrix);

  graph g;
  gen_graph(g, PAM_matrix, s1, s2);

  auto score = traverse_path(g, g.nodes.find(to_key(s1.length(), s2.length())));
  auto ret = backtrace(g, g.nodes.find(to_key(s1.length(), s2.length())), s1, s2);

  cout << comp_score(ret.first, ret.second, PAM_matrix) << endl;

  return make_tuple(score, ret.second, ret.first);
}