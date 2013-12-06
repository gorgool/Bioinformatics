#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <list>
#include "graph.h"

using namespace std;

static list<map<string, node>::iterator> traverse_tour(map<string, node>::iterator begin_node)
{
  list<map<string, node>::iterator> ret;
  bool end_tour = false;
  while (!end_tour)
  {
    end_tour = true;
    ret.push_back(begin_node);

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
    
  ret.erase(--ret.end());

  return ret;
}

static map<string, node>::iterator find_insertion_point(graph& g, list<map<string, node>::iterator>& nodes)
{
  for (auto p = nodes.begin(); p != nodes.end(); ++p)
  {
    for (const auto& ed : (*p)->second.output_edges)
    {
      if (ed.visited == false)
        return *p;
    }
  }
  return g.nodes.end();
}

list<map<string, node>::iterator> find_eulerian_cycle(graph& g)
{
  list<map<string, node>::iterator> ret;

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<size_t> rang(0, g.nodes.size() - 1);

  map<string, node>::iterator begin_node = g.nodes.begin();
  advance(begin_node, rang(gen));

  while (true)
  {
    if (begin_node == g.nodes.end())
      break;

    auto tour = traverse_tour(begin_node);

    if (ret.empty())
    {
      ret = tour;
      begin_node = find_insertion_point(g, ret);
      continue;
    } 

    auto rotation_point = find_if(ret.begin(), ret.end(),
      [&](const map<string, node>::iterator it)
    {
      return it->first == begin_node->first;
    });
    ret.insert(rotation_point, tour.begin(), tour.end());

    begin_node = find_insertion_point(g, ret);
  }

  return ret;
}

static list<map<string, paired_node>::iterator> traverse_tour(map<string, paired_node>::iterator begin_node)
{
  list<map<string, paired_node>::iterator> ret;
  bool end_tour = false;
  while (!end_tour)
  {
    end_tour = true;
    ret.push_back(begin_node);

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

  ret.erase(--ret.end());

  return ret;
}

static map<string, paired_node>::iterator find_insertion_point(paired_graph& g, list<map<string, paired_node>::iterator>& nodes)
{
  for (auto p = nodes.begin(); p != nodes.end(); ++p)
  {
    for (const auto& ed : (*p)->second.output_edges)
    {
      if (ed.visited == false)
        return *p;
    }
  }
  return g.nodes.end();
}

list<map<string, paired_node>::iterator> find_eulerian_cycle(paired_graph& g)
{
  list<map<string, paired_node>::iterator> ret;

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<size_t> rang(0, g.nodes.size() - 1);

  map<string, paired_node>::iterator begin_node = g.nodes.begin();
  advance(begin_node, rang(gen));

  while (true)
  {
    if (begin_node == g.nodes.end())
      break;

    auto tour = traverse_tour(begin_node);

    if (ret.empty())
    {
      ret = tour;
      begin_node = find_insertion_point(g, ret);
      continue;
    }

    auto rotation_point = find_if(ret.begin(), ret.end(),
      [&](const map<string, paired_node>::iterator it)
    {
      return it->first == begin_node->first;
    });
    ret.insert(rotation_point, tour.begin(), tour.end());

    begin_node = find_insertion_point(g, ret);
  }

  return ret;
}