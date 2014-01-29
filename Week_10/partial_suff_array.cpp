#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<size_t> construct_suffix_array(const string& text);

vector<pair<size_t, size_t>> partial_suff_array(const string& text, const size_t k)
{
  vector<pair<size_t, size_t>> ret;

  auto suff_array = construct_suffix_array(text);

  size_t val = 0;
  while (val < text.length())
  {
    auto it = find(suff_array.begin(), suff_array.end(), val);
    ret.push_back(make_pair(distance(suff_array.begin(), it), val));
    val += k;
  }

  sort(ret.begin(), ret.end(), [](const pair<size_t, size_t>& lhs, const pair<size_t, size_t>& rhs){ return lhs.first < rhs.first; });

  return ret;
}