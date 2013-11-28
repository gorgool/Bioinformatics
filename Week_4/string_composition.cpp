#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<string> composition(const string& text, const size_t k)
{
  vector<string> ret;

  for (size_t pos = 0; pos < text.length() - k + 1; pos++)
  {
    ret.push_back(text.substr(pos, k));
  }

  sort(ret.begin(), ret.end());

  return ret;
}