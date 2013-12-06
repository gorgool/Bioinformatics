#include <vector>
#include <string>
#include <iostream>
#include <list>
#include "graph.h"

using namespace std;

list<map<string, node>::iterator> find_eulerian_cycle(graph& g);
list<map<string, paired_node>::iterator> find_eulerian_path(paired_graph& g);

string string_reconstruct(graph& g, const size_t k)
{
  list<map<string, node>::iterator> strings = find_eulerian_cycle(g);

  // Cycle the path
  strings.push_back(*strings.begin());

  auto it = strings.begin();
  string ret((*it)->first);

  while (true)
  {
    if (distance(it, strings.end()) > (*strings.begin())->first.size())
    {
      advance(it, (*strings.begin())->first.size());
      ret += (*it)->first;
      continue;
    }
    
    auto last = (*strings.rbegin())->first;
    ret += string(last.begin() + last.size() - distance(it, strings.end()) + 1, last.end());
    break;
  }

  ret.resize(ret.size() - k + 1);

  return ret;
}

string string_reconstruct(paired_graph& g, const size_t k, const size_t len)
{
  list<map<string, paired_node>::iterator> strings = find_eulerian_path(g);

  auto it = strings.begin();
  string ret[2];
  ret[0] = ((*it)->second.label[0]);
  ret[1] = ((*it)->second.label[1]);

  while (true)
  {
    if (distance(it, strings.end()) > (*strings.begin())->second.label[0].size())
    {
      advance(it, (*strings.begin())->second.label[0].size());
      ret[0] += (*it)->second.label[0];
      ret[1] += (*it)->second.label[1];
      continue;
    }

    auto last = (*strings.rbegin())->second.label[0];
    ret[0] += string(last.begin() + last.size() - distance(it, strings.end()) + 1, last.end());
    last = (*strings.rbegin())->second.label[1];
    ret[1] += string(last.begin() + last.size() - distance(it, strings.end()) + 1, last.end());
    break;
  }

  return ret[0] + ret[1].substr(ret[1].size() - k - len);
}