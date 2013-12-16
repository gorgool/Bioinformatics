#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include "graph.h"
#include <algorithm>

using namespace std;

vector<string> split_string(const string& s);
string to_key(const size_t i, const size_t j);

static void gen_matrix(map<string, int>& m)
{
  string matrix_file = "blosum.dat";
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
      if (i < s1.length() && j < s2.length())
      {
        g.nodes[to_key(i + 1, j + 1)];
        g.nodes[to_key(i, j + 1)];
        g.nodes[to_key(i + 1, j)];
        g.nodes[to_key(i, j)];

        string key{ s1[i], s2[j] };

        // Matches
        g.nodes[to_key(i, j)].output_edges.push_back(edge( m[key], g.nodes.find(to_key(i + 1, j + 1)), matches, s1[i] == s2[j] ? s1[i] : s2[j]));
        // Insertion
        g.nodes[to_key(i, j)].output_edges.push_back(edge( sigma, g.nodes.find(to_key(i + 1, j)), insertion));
        // Deletion
        g.nodes[to_key(i, j)].output_edges.push_back(edge( sigma, g.nodes.find(to_key(i, j + 1)), deletion, '-'));

        // Matches
        g.nodes[to_key(i + 1, j + 1)].input_edges.push_back(edge(m[key], g.nodes.find(to_key(i, j)), matches, s1[i]));
        // Insertion
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge( sigma, g.nodes.find(to_key(i, j)), insertion));
        // Deletion
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), deletion, '-'));
      }

      if (i == s1.length() && j != s2.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge( sigma, g.nodes.find(to_key(i, j + 1)), deletion));
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge( sigma, g.nodes.find(to_key(i, j)), insertion));
      }

      if (j == s2.length() && i != s1.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge( sigma, g.nodes.find(to_key(i + 1, j)), insertion));
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge( sigma, g.nodes.find(to_key(i, j)), deletion));
      }
    }
  }
}

static map<string, int> memo;

static int traverse_path(graph& g, map<string, node>::iterator n)
{
  int ret = 0;
  vector<pair<vector<edge>::iterator, int>> weights;
  for (auto input_edge = n->second.input_edges.begin(); input_edge != n->second.input_edges.end(); ++input_edge)
  {
    auto key = input_edge->to->first;
    if (memo.find(key) == memo.end())
      memo[key] = traverse_path(g, input_edge->to);

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

  n->second.value = ret;
  return ret;
}

static pair<string, string> backtrace(const graph& g, map<string, node>::iterator n)
{
  auto it = n;
  string ret1, ret2;
  while (true)
  {
    for (const auto& e : it->second.input_edges)
    {
      if (e.visited == true)
      {
        switch (e.type)
        {
          case deletion:
          {   
            ret1 += e.label;
            ret2 += '-';
            break;
          } 
          case matches:
          {  
            ret1 += e.label;
            ret2 += e.label;
            break;
          }
          case insertion:
          {        
            ret2 += e.label;
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

  return make_pair(ret1, ret2);
}

static int compute_score(string& s1, string& s2, map<string, int>& m)
{
  int score = 0;
  for (size_t i = 0; i < s1.length(); i++)
  {
    if (s1[i] == '-' || s2[i] == '-')
    {
      score += -5;
      continue;
    }

    string key{ s1[i], s2[i] };
    score += m[key];
  }
  return score;
}

tuple<int, string, string> global_alignment(string& s1, string& s2)
{
  if (s1.length() < s2.length())
    swap(s1, s2);

  map<string, int> BLOSUM_matrix;
  gen_matrix(BLOSUM_matrix);
  graph g;
  gen_graph(g, BLOSUM_matrix, s1, s2);

  auto score = traverse_path(g, g.nodes.find(to_key(s1.length(), s2.length())));
  auto str = backtrace(g, g.nodes.find(to_key(s1.length(), s2.length())));

  //string ttt = "FTFRECMQSCGLYKYIRETQDKDGGKGCQPENIITPPICQVDCLQIRSPQSWIVTSWNYLHLNCFGFIQWRCAVWKNVYTEKKDKQITWHNHLLDIVGECVVALNRQHMMWDYDVHFYIMGSRYAYESDFYLELNNGHYLEAVMFAHQQQYPMDAFSEIKDFLIKFDVGKARRSGFMVVMHHSTDYTGWDCMFEVEHTCWREPKHLGCGMHQPNRYHAFHTKLPFHTDFHFYKSINHVNKCVAKYWKACSGYPGCPKYGSELPLRYGGKPTPGETWHHAAFWPLDLYSWRWFILCTGTWRTMFIVKFFHREDGTPTKGYEEVIHIHIRMTQNIGIQHVTCGRESDMKLNHFCLQCAVEGHEQDVGMKTVARADQKDLLWCVKLSVSMMNAAGEAYMLNECCMVHQCCGLTWPSSFKFFYNSCLYSVNMAHHINNLDVVLRTHEAVSDLPMYCQSMQLITWETSNSRWRTMWKYNCQCKRWMTLTFIVKMRPSKFFRGQDYMGETERFHHIFADVIGEAATNHVTCGFLDYFLYVNETIDMLRTYASMTCPCRWELWYVDKIWLWTPAPIFMSWCCTYRNWGSTCNAGTCAFMCNGWLRKDQQKWMTSWDIIDAKDSVMQFQNCICVRNFRIQCKCTDMWQRDMCMHLIHHGRVNTCIYRSSKKPHGMLILKHYGDRPLGKEQTYTPTHGPRKCMVTYNMQYLKQVYINMSRHTAGDRMEYNCTEFWHHLFMVVGQDDQWMMAMIADYPVKGFHLPSYVYLYLKDKHQDIWHKQQLDRHSDIFICTVVQHNSMDKVHIHRPLGETMKYELTFFIFWCNHKMELQHADRYPLHPSDAHIGVQECHALMWTCVMTWFGCVGTLHYFVASCQMAKWIGKEGRLSYMGACLDWFYYHIPWSRDLCYAAIIWYAMKRPIMDNQIPWTLHACERWMD";
  //string yyy = "FTVQH-LELLGLYKYIRETQ--E--KGCQPEMVI-GSWCKLDCLQIRSDQLWIVTSWNY-H-H--GFIQWRCAVCKNVCTEKYDKQITWHNHLLDIVGECVVALNRQHMMW--DVRLDI--N-YAYESDFYLELNNGHTLEAV------P-PMDAFSEIKDFKIKFDVGKARRSGFMVVMHHS---S--DCMFEYEHTCWREPKKLICGMHQYNRYHAFVTKLPFHTDFHFSKQINH----L--F-KMCSGY---P-V--ELHLRY--IPTPGETWHHAAF-ALNIYSWRWFIL---HWRTMHIVKFFHHEDQTPTKGCEEVFRIHSRMTQNFDFQNIHEGADNELQTNEFYRFCAVCGHEQDVGMKYYARAG-K---WCVKLSVSMMDAAGEAIMLNECCMVHQCCGLTWPSSFKFF---MLNSVNMAHHILNLDVVLRTHSAVSDLPMYCHSMQLITQQKSRMAIETSLKYRLHANCSMTYNFIVK-E-S-MYK---FLGELDT---VFDDVIGEAATNHVTCGFLDYFLHVNETIDMY-NYV-I------ELWYV-SIWLWTPAPQFMSWCCIHREWWQICNVGTCE---T--LRKDQQIW-PGTNKMKWKWDII-FQNCICRRDFRSRTKCWIMCQRDMCM---HY----TCIDRSSKHPHGELVLKHYGDRPLGKEQTYTPTHGPRKCMVTYNMQYLPQVYIAMSR--VGDRMEYNCCEF--PL---VGQDDQWMMAMIADYPVKIFHLPSYVYCCSRYYHR-LYLKDKSDYHKQIFICTVVQHNSMDKVHIHRPLG-S--YELTSFIFWCNHKMELQHADRYPLHPSDAHIGVQECHALMWTCVI-WFGCVGTLHYFVAPCQMAKWIGKEGRLSYAGA--K--AY-MP-PQTVCDAAIA-H-M---VY-HMFPWTLHACERWMD";

  //int s = compute_score(ttt, yyy, BLOSUM_matrix);

  return make_tuple(score, str.first, str.second);
}