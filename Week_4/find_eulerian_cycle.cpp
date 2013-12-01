#include <vector>
#include <string>
#include <random>
#include "graph.h"

using namespace std;

static vector<string> traverse_tour(graph& _graph, list<node>::iterator begin_node)
{
  vector<string> ret;
  bool end_tour = false;
  while (!end_tour)
  {
    end_tour = true;
    ret.push_back(begin_node->data);
    auto& o_edges = begin_node->output_edges;
    for (auto& item : o_edges)
    {
      if (item.visited == false)
      {
        item.visited = true;
        begin_node = item.to;
        end_tour = false;
        break;
      }
    }
  }

  if (*ret.begin() == *ret.rbegin())
    ret.resize(ret.size() - 1);

  return ret;
}

static vector<string>::iterator find_rotation_point(graph& _graph, vector<string>& nodes)
{
  for (auto p = nodes.begin(); p != nodes.end(); ++p)
  {
    auto it = _graph.find_node(*p);
    auto& o = it->output_edges;
    for (const auto& ed : o)
    {
      if (ed.visited == false)
        return p;
    }
  }
  return nodes.end();
}

vector<string> find_eulerian_cycle(graph& _graph)
{
  vector<string> ret;

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<size_t> rang(0, _graph.nodes.size() - 1);

  list<node>::iterator begin_node = _graph.nodes.begin();
  advance(begin_node, rang(gen));

  while (true)
  {
    auto tour = traverse_tour(_graph, begin_node);

    ret.insert(ret.end(), tour.begin(), tour.end());

    auto rotation_point = find_rotation_point(_graph, ret);

    if (rotation_point == ret.end())
      break;

    begin_node = _graph.find_node(*rotation_point);
    rotate(ret.begin(), rotation_point, ret.end());
  }
 
  ret.push_back(*ret.begin());
  return ret;
}