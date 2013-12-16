#include <string>
#include <vector>
#include "graph.h"
#include <algorithm>
#include <numeric>

using namespace std;

vector<string> split_string(const string& s);

void gen_graph(graph& g, const vector<string>& node_list)
{
  for (const auto& s : node_list)
  {
    auto tmp = split_string(s);

    g.nodes[tmp[1]];
    g.nodes[tmp[0]].output_edges.push_back(edge( stoi(tmp[2]), g.nodes.find(tmp[1])));
    g.nodes[tmp[1]].input_edges.push_back(edge(stoi(tmp[2]), g.nodes.find(tmp[0])));
  }
}

static map<string, int> memo;

static int traverse_path(graph& g, const map<string, node>::iterator sink, const map<string, node>::iterator source)
{
  int ret = 0;
  vector<pair<vector<edge>::iterator, int>> weights;

  for (auto input_edge = sink->second.input_edges.begin(); input_edge != sink->second.input_edges.end(); ++input_edge)
  {
    auto key = input_edge->to->first;
    if (memo.find(key) == memo.end())
      memo[key] = traverse_path(g, input_edge->to, source);

    weights.push_back({ input_edge, input_edge->weight + memo[key] });
  }

  if (!weights.empty())
  {
    auto it = max_element(weights.begin(), weights.end(),
      [](const pair<vector<edge>::iterator, int>& lhs, const pair<vector<edge>::iterator, int>& rhs)
    {
      return lhs.second < rhs.second;
    });

    ret = it->second;
    it->first->visited = true;
  }

  sink->second.value = ret;
  return ret;
}

static pair<int, vector<int>> backtrace(const graph& g, const map<string, node>::iterator sink, const map<string, node>::iterator source)
{
  auto it = sink;
  vector<int> ret;
  vector<int> score;
  while (true)
  {
    for (const auto& e : it->second.input_edges)
    {
      if (e.visited == true)
      {
        ret.push_back(stoi(e.to->first));
        score.push_back(e.weight);
        it = e.to;
        break;
      }
    }

    if (it->second.input_edges.empty() || it == source)   
      break;    
  }

  ret.resize(ret.size() - 1);

  return make_pair(accumulate(score.begin(), score.end() - 1, 0), vector<int>(ret.rbegin(), ret.rend()));
}

pair<int, vector<int>> find_longes_path_DAG(const size_t source_node, const size_t sink_node, const vector<string>& node_list)
{
  graph g;
  gen_graph(g, node_list);

  traverse_path(g, g.nodes.find(to_string(sink_node)), g.nodes.find(to_string(source_node)));
  auto inter_path = backtrace(g, g.nodes.find(to_string(sink_node)), g.nodes.find(to_string(source_node)));

  vector<int> path;
  path.push_back(source_node);
  path.insert(path.end(), inter_path.second.begin(), inter_path.second.end());
  path.push_back(sink_node);

  return { inter_path.first, path };
}