#include <vector>

using namespace std;

size_t num_break_points(const vector<int> in_seq)
{
  vector<int> in{ 0 };
  in.insert(in.end(), in_seq.begin(), in_seq.end());
  in.push_back(in_seq.size() + 1);

  size_t ret = 0;

  for (size_t i = 0; i < in.size() - 1; i++)
  {
    if (in[i + 1] - in[i] != 1)
      ret++;
  }

  return ret;
}