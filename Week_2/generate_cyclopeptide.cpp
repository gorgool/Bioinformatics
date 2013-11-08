#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

static vector<set<size_t>> next_seq(set<size_t> start_seq, size_t specturm_length, size_t step)
{
  vector<set<size_t>> ret;

  for (size_t i = 0; i < specturm_length; i++)
  {
      set<size_t> new_seq(start_seq);
      new_seq.insert(i);

      if (new_seq.size() == step)
        ret.push_back(new_seq);
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

  vector<set<size_t>> list{ {0} };

  list = next_seq(list[0], spec.size(), 2);

  list = next_seq(list[5], spec.size(), 3);

  list = next_seq(list[2], spec.size(), 4);
  
  return vector<vector<size_t>>();
}