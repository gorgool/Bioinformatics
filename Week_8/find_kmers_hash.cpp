#include <string>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

static map<char, char> pairs =
{
  make_pair('A', 'T'),
  make_pair('T', 'A'),
  make_pair('C', 'G'),
  make_pair('G', 'C')
};

static string reverse_comp(const string& s)
{
  string ret(s);
  for (size_t i = 0; i < s.length(); i++)
  {
    ret[i] = pairs[s[i]];
  }
  return string(ret.rbegin(), ret.rend());
}

vector<pair<size_t, size_t>> find_kmers_hash(const size_t k, const string& s1, const string& s2)
{
  vector<pair<size_t, size_t>> ret;
  map<string, vector<size_t>> kmer_tbl;

  for (size_t i = 0; i <= s1.length() - k; i++)
  {
    auto substr = s1.substr(i, k);
    kmer_tbl[substr].push_back(i);
  }

  for (size_t i = 0; i <= s2.length() - k; i++)
  {
    auto substr = s2.substr(i, k);
    if (kmer_tbl.find(substr) != kmer_tbl.end())
    {
      for (const auto& idx : kmer_tbl[substr])
        ret.push_back(make_pair(idx, i));
    }

    auto rev_substr = reverse_comp(substr);
    if (kmer_tbl.find(rev_substr) != kmer_tbl.end())
    {
      for (const auto& idx : kmer_tbl[rev_substr])
        ret.push_back(make_pair(idx, i));
    }
  }

  return ret;
}