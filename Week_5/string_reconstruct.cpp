#include <vector>
#include <string>
#include <iostream>
#include <list>
#include "graph.h"

using namespace std;

list<map<string, node_m>::iterator> find_eulerian_cycle(graph_m& g);

string string_reconstruct(graph_m& g, const size_t k)
{
  list<map<string, node_m>::iterator> strings = find_eulerian_cycle(g);

  // Loop the path
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