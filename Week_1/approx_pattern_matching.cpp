#include <string>
#include <vector>
#include <cassert>

using namespace std;

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

vector<size_t> approx_pattern_matching(const string& input_str, const string& pattern, size_t k)
{
  vector<size_t> ret;

  for (size_t i = 0; i <= input_str.length() - pattern.length(); i++)
  {
    string substr = input_str.substr(i, pattern.length());
    if (max_num_misses(substr, pattern) <= k)
      ret.push_back(i);
  }

  return ret;
}