#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

vector<size_t> construct_suffix_array(const string& text)
{
  vector<size_t> ret(text.length());
  size_t from = 0;
  generate_n(ret.begin(), text.length(), [&from](){ return from++; });

  sort(ret.begin(), ret.end(), [&text](const size_t lhs, const size_t rhs)
  {
    return strcmp(text.c_str() + lhs, text.c_str() + rhs) < 0;
  });

  return ret;
}