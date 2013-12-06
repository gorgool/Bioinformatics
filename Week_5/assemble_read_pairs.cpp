#include <vector>
#include "graph.h"
#include <string>
#include <list>

using namespace std;

string string_reconstruct(paired_graph& g, const size_t k, const size_t len);

static void gen_graph(vector<pair<string, string>>& pairs, paired_graph& g)
{
  for (const auto& item : pairs)
  {
    string suffix[2] { item.first.substr(1), item.second.substr(1) };
    string prefix[2] { item.first.substr(0, item.first.size() - 1), item.second.substr(0, item.second.size() - 1) };

    auto& to = g.nodes[suffix[0] + suffix[1]];
    to.label[0] = suffix[0];
    to.label[1] = suffix[1];


    auto& from = g.nodes[prefix[0] + prefix[1]];
    from.label[0] = prefix[0];
    from.label[1] = prefix[1];

    paired_edge e{};
    e.name[0] = item.first;
    e.name[1] = item.second;
    e.to = g.nodes.find(suffix[0] + suffix[1]);

    from.output_edges.emplace_back(e);
  }
}

string assemble_read_pairs(vector<pair<string, string>>& pairs, const size_t k)
{
  paired_graph g;
  gen_graph(pairs, g);

  auto ret = string_reconstruct(g, k, pairs[0].first.size());

  return ret;
}