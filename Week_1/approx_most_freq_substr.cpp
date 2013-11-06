#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>

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
  assert(input.length() == original.length(), "Error. Length of input strings must be equal.");

  size_t num = 0;
  for (size_t i = 0; i < input.length(); i++)
  {
    if (input[i] != original[i])
      num++;
  }

  return num;
}

vector<string> approx_most_freq_substr(const string& input_str, size_t k, size_t d)
{
  vector<string> tbl;

  for (size_t i = 0; i <= input_str.size() - k; i++)
  {
    string substr = input_str.substr(i, k);
    tbl.push_back(substr);
  }

  string pattern(k, 'A');
  size_t max_score = 0;
  vector<string> ret;
  for (size_t i = 0; i < pow(4, k) - 1; i++)
  {
    size_t score = 0;

    for (const auto& entry : tbl)
    {
      if (max_num_misses(pattern, entry) <= d)
        score++;
    }

    if (score >= max_score && score > 0)
    {
      if (score == max_score)
        ret.push_back(pattern);
      else
      {
        ret.clear();
        ret.push_back(pattern);
      }
      max_score = score;
    }

    pattern = next_pattern(pattern);
  }

  return ret;
}