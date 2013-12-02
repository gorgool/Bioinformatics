#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <list>
#include "graph.h"

using namespace std;

static list<map<string, node_m>::iterator> traverse_tour(graph_m& _graph, map<string, node_m>::iterator begin_node)
{
  list<map<string, node_m>::iterator> ret;
  bool end_tour = false;
  while (!end_tour)
  {
    end_tour = true;
    ret.push_front(begin_node);

    for (auto& item : begin_node->second.output_edges)
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

  if ((*ret.begin())->first == (*ret.rbegin())->first)
    ret.erase(--ret.end());

  return ret;
}

static map<string, node_m>::iterator find_insertion_point(graph_m& _graph, list<map<string, node_m>::iterator>& nodes)
{
  for (auto p = nodes.begin(); p != nodes.end(); ++p)
  {
    for (const auto& ed : (*p)->second.output_edges)
    {
      if (ed.visited == false)
        return *p;
    }
  }
  return _graph.nodes.end();
}

list<map<string, node_m>::iterator> find_eulerian_cycle(graph_m& _graph)
{
  list<map<string, node_m>::iterator> ret;

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<size_t> rang(0, _graph.nodes.size() - 1);

  map<string, node_m>::iterator begin_node = _graph.nodes.begin();
  advance(begin_node, rang(gen));

  while (true)
  {
    auto tour = traverse_tour(_graph, begin_node);

    if (ret.empty())
    {
      ret = tour;
      begin_node = find_insertion_point(_graph, ret);
      continue;
    } 

    auto insertion_point = find_insertion_point(_graph, ret);
    if (insertion_point == _graph.nodes.end())
      break;

    begin_node = insertion_point;

    auto rotation_point = find_if(ret.begin(), ret.end(),
      [&](const map<string, node_m>::iterator it)
    {
      return it->first == insertion_point->first;
    });
    ret.insert(rotation_point, tour.begin(), tour.end());

  }

  return ret;
}