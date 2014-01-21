#include <string>
#include <vector>

using namespace std;

string BWT_to_string(const string& bwt);

vector<size_t> BWT_matching(const string& bwt, const vector<string>& patterns)
{
  auto s = BWT_to_string(bwt);
  return vector<size_t>();
}