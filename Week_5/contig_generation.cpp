#include <vector>
#include <string>
#include "graph.h"

using namespace std;

static vector<vector<map<string, node>::iterator>> traverse_to_branch(const map<string, node>::iterator begin_node)
{
  vector<vector<map<string, node>::iterator>> ret;
  // For all output paths
  for (const auto& o : begin_node->second.output_edges)
  {
    vector<map<string, node>::iterator> path;

    path.push_back(begin_node);
    auto it = o.to;

    while (true)
    {
      path.push_back(it);

      if (it->second.output_edges.size() != 1 || it->second.input_edges.size() != 1)
        break;

      it = it->second.output_edges[0].to;
    }

    ret.push_back(path);
  }
  
  return ret;
}

static string string_gen(const vector<map<string, node>::iterator>& path)
{
  auto it = path.begin();
  string ret((*it)->first);

  while (true)
  {
    if (distance(it, path.end()) > (*path.begin())->first.size())
    {
      advance(it, (*path.begin())->first.size());
      ret += (*it)->first;
      continue;
    }

    auto last = (*path.rbegin())->first;
    ret += string(last.begin() + last.size() - distance(it, path.end()) + 1, last.end());
    break;
  }

  return ret;
}

vector<string> contig_generation(const vector<string>& v)
{
  graph g;

  for (const auto& item : v)
  {
    auto lhs = item.substr(0, item.size() - 1);
    auto rhs = item.substr(1);

    g.nodes[lhs];
    g.nodes[rhs];

    g.nodes[lhs].output_edges.push_back(edge{ "", g.nodes.find(rhs) });
    g.nodes[rhs].input_edges.push_back(edge{ "", g.nodes.find(lhs) });
  }

  vector<string> ret;

  for (auto it = g.nodes.begin(); it != g.nodes.end(); ++it)
  {
    // For nodes without input edges
    if (it->second.input_edges.empty())
    {
      auto paths = traverse_to_branch(it);
      for (const auto& path : paths)
        ret.push_back(string_gen(path));
    }

    // For nodes such that input and output edges not equal 1
    if (
      it->second.input_edges.size() >= 1 && 
      it->second.output_edges.size() >= 1 && 
      (it->second.input_edges.size() + it->second.output_edges.size()) > 2)
    {
      auto paths = traverse_to_branch(it);
      for (const auto& path : paths)
        ret.push_back(string_gen(path));
    }
  }

  return ret;
}
