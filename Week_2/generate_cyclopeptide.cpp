#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <numeric>
#include <unordered_set>

using namespace std;

static vector<unordered_set<size_t>> expand(const unordered_set<size_t> start_seq, const size_t specturm_length, const size_t step)
{
  vector<unordered_set<size_t>> ret;

  for (size_t i = 0; i < specturm_length; i++)
  {
    unordered_set<size_t> new_seq(start_seq);
      new_seq.insert(i);

      if (new_seq.size() == step)
        ret.push_back(new_seq);
  }

  return ret;
}

static vector<unordered_set<size_t>> filter_seq(vector < unordered_set < size_t >> &list, vector<size_t>& mass_spectrum, map<size_t, size_t>& mass_tbl)
{
  vector<unordered_set<size_t>> ret;
  for (const auto& seq : list)
  {
    size_t sum = 0;
    for (auto mass_idx : seq)
      sum += mass_tbl[mass_idx];

    if (binary_search(mass_spectrum.begin(), mass_spectrum.end(), sum))
      ret.push_back(seq);
  }

  return ret;
}

vector<vector<size_t>> generate_cyclopeptide(vector<size_t>& mass_spectrum)
{
  vector<size_t> spec;
  for (auto mass : mass_spectrum)
  {
    if (mass > 186)
      break;

    if (mass !=0)
      spec.push_back(mass);
  }
  
  map<size_t, size_t> mass_tbl;

  for (size_t i = 0; i < spec.size(); i++)
    mass_tbl.insert({ i, spec[i] });

  vector<unordered_set<size_t>> list{ {0} };

  sort(mass_spectrum.begin(), mass_spectrum.end());
  for (size_t i = 2; i <= spec.size(); i++)
  {
    list = filter_seq(list, mass_spectrum, mass_tbl);

    vector<unordered_set<size_t>> new_list;
    for (const auto& seq : list)
    {
      vector<unordered_set<size_t>> new_seqs = expand(seq, spec.size(), i);

      for (const auto& new_seq : new_seqs)
        new_list.push_back(new_seq);
    }

    list.resize(new_list.size());
    copy(new_list.begin(), new_list.end(), list.begin());
  }
  
  return vector<vector<size_t>>();
}