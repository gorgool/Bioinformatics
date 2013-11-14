#include <vector>
#include <algorithm>
#include <map>
#include <cassert>

using namespace std;

vector<size_t> leaderboard_sequencing(vector<size_t>& mass_spectrum, const size_t N, vector<size_t>& mass_list);
vector<size_t> spectrum_convolution(vector<size_t>& mass_spectrum);

vector<size_t> get_spectrum(vector<size_t>& mass_spectrum, const size_t M)
{
  vector<size_t> spec_conv = spectrum_convolution(mass_spectrum);
  sort(spec_conv.begin(), spec_conv.end());

  assert(!spec_conv.empty());

  vector<pair<size_t, size_t>> freq_tbl{ { spec_conv[0] , 0} };

  auto prev_entry = spec_conv[0];
  size_t idx = 0;

  for (auto entry : spec_conv)
  {
    if (entry < 57 || entry > 200)
      continue;

    if (entry == prev_entry)
      freq_tbl[idx].second++;
    else
    {
      freq_tbl.push_back({ entry, 1 });
      idx++;
    }
    prev_entry = entry;
  }

  sort(freq_tbl.begin(), freq_tbl.end(), [](const pair<size_t, size_t>& lhs, const pair<size_t, size_t>& rhs)
  {
    return lhs.second > rhs.second;
  });

  if (M < freq_tbl.size())
    freq_tbl.resize(M);

  vector<size_t> ret;
  for (const auto& entry : freq_tbl)
    ret.push_back(entry.first);

  return ret;
}

vector<size_t> convolution_leaderboard_sequencing(vector<size_t>& mass_spectrum, const size_t M, const size_t N)
{
  vector<size_t> mass_list = get_spectrum(mass_spectrum, M);
  return leaderboard_sequencing(mass_spectrum, N, mass_list);
}