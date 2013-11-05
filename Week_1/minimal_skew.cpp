#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<size_t> minimal_skew(const string& input_str)
{
  vector<int> sum;
  int current_sum = 0;

  for (const auto& ch : input_str)
  {
    sum.push_back(current_sum);

    if (ch == 'C')
      current_sum -= 1;

    if (ch == 'G')
      current_sum += 1;
  }
  sum.push_back(current_sum);

  auto min_iter = min_element(sum.begin(), sum.end());

  vector<size_t> ret;

  for (size_t i = 0; i < sum.size(); ++i)
  {
    if (sum[i] == *min_iter)
      ret.push_back(i);
  }

  return ret;
}