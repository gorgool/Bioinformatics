#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <set>

using namespace std;

// Pattern generator
map<char, char> alphabeth{ { 'A', 'T' }, { 'T', 'C' }, { 'C', 'G' }, { 'G', 'A' } };

static string next_pattern(const string& prev_pattern)
{
  string ret(prev_pattern);
  for (size_t i = prev_pattern.length() - 1; i >= 0; --i)
  {
    if (ret[i] != 'G')
    {
      ret[i] = alphabeth[ret[i]];
      break;
    }
    else
      ret[i] = alphabeth[ret[i]];
  }
  return ret;
}

static size_t max_num_misses(const string& input, const string& original)
{
  assert(input.length() == original.length());

  size_t num = 0;
  for (size_t i = 0; i < input.length(); i++)
  {
    if (input[i] != original[i])
      num++;
  }

  return num;
}

vector<string> motif_enum(const vector<string>& dna, const size_t k, const size_t d)
{
  string pattern(k, 'A');

  set<string> ret;

  for (size_t i = 0; i < pow(4, k) - 1; i++)
  {
    vector<bool> exist(dna.size(), false);
    for (size_t pos = 0; pos < dna[0].length() - k + 1; pos++)
    {
      for (size_t j = 0; j < dna.size(); ++j)
      {
        string substr = dna[j].substr(pos, k);
        if (max_num_misses(substr, pattern) <= d)
          exist[j] = true;
      }     
    }

    // if true for all dna strings
    bool check = true;
    for (auto item : exist)
      check = check && item;

    if (check)
      ret.insert(pattern);

    pattern = next_pattern(pattern);
  }

  return vector<string>(ret.begin(), ret.end());
}
