#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <numeric>

using namespace std;

extern map<char, char> alphabeth;

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

string median_string(const vector<string>& dna, const size_t k)
{
  string pattern(k, 'A');

  size_t min_dist = numeric_limits<size_t>::max();
  string ret;

  for (size_t i = 0; i < pow(4, k) - 1; i++)
  {
    vector<size_t> score(dna.size(), numeric_limits<size_t>::max());
    for (size_t pos = 0; pos < dna[0].length() - k + 1; pos++)
    {
      for (size_t j = 0; j < dna.size(); ++j)
      {
        string substr = dna[j].substr(pos, k);
        size_t dist = max_num_misses(pattern, substr);
        if (dist < score[j])
          score[j] = dist;
      }
    }

    if (accumulate(score.begin(), score.end(), 0) < min_dist)
    {
      ret = pattern;
      min_dist = accumulate(score.begin(), score.end(), 0);
    }     

    pattern = next_pattern(pattern);
  }

  return ret;
}