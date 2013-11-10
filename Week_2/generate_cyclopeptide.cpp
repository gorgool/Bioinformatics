#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_set>

using namespace std;

static vector<vector<size_t>> expand(const vector<size_t>& start_seq, const size_t specturm_length, const size_t step)
{
  vector<vector<size_t>> ret;

  for (size_t i = 0; i < specturm_length; i++)
  {
    vector<size_t> new_seq(start_seq);

    bool exist(false);
    for (const auto mass_idx : new_seq)
    {
      if (i == mass_idx)
      {
        exist = true;
        break;
      }
    }  

    if (exist == false)
      new_seq.push_back(i);

    if (new_seq.size() == step)
      ret.push_back(move(new_seq));
  }

  return ret;
}

map<size_t, char> peptide_tbl
{
	{ 57, 'G' },
	{ 71, 'A' },
	{ 87, 'S' },
	{ 97, 'P' },
	{ 99, 'V' },
	{ 101, 'T' },
	{ 103, 'C' },
	{ 113, 'I' },
	{ 114, 'N' },
	{ 115, 'D' },
	{ 128, 'K' },
	{ 129, 'E' },
	{ 131, 'M' },
	{ 137, 'H' },
	{ 147, 'F' },
	{ 156, 'R' },
	{ 163, 'Y' },
	{ 186, 'W' }
};

extern map<char, size_t> mass_tbl;

static size_t compute_mass(const string& proteins)
{
  size_t total_mass = 0;
  for (auto protein : proteins)
  {
    total_mass += mass_tbl[protein];
  }

  return total_mass;
}

static vector<size_t> generate_spectrum(const string& proteins)
{
  vector<size_t> ret;

  ret.push_back(0);

  for (size_t i = 1; i < proteins.length(); i++)
  {
    for (size_t j = 0; j < proteins.length(); j++)
    {
      if (i + j <= proteins.length())
      {
        string substr = proteins.substr(j, i);
        ret.push_back(move(compute_mass(substr)));
      }
    }
  }
  ret.push_back(move(compute_mass(proteins)));

  return ret;
}


static vector<vector<size_t>> filter_seq(vector<vector<size_t>> &list, vector<size_t>& mass_spectrum, map<size_t, size_t>& mass_tbl)
{
  vector<vector<size_t>> ret;

  for (const auto& seq : list)
  {
    // Turn seq into a peptide
	  string peptide;
    for (auto mass_idx : seq)
      peptide += peptide_tbl[mass_tbl[mass_idx]];

    vector<size_t> peptide_spec = generate_spectrum(peptide);

    size_t counter = 0;
    for (const auto mass : peptide_spec)
    {
      if (binary_search(mass_spectrum.begin(), mass_spectrum.end(), mass))
        counter++;
    }

    if (counter == peptide_spec.size())
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
  vector<vector<size_t>> list;

  for (size_t i = 0; i < spec.size(); i++)
  {
    mass_tbl.insert({ i, spec[i] });
    list.push_back({ i });
  }

  // For binary search in filter_seq function
  sort(mass_spectrum.begin(), mass_spectrum.end());

  for (size_t i = 2; i <= spec.size(); i++)
  {
    vector<vector<size_t>> new_list;

    for (const auto& seq : list)
    {
      vector<vector<size_t>> new_seqs = expand(seq, spec.size(), i);

      for (const auto& new_seq : new_seqs)
        new_list.push_back(move(new_seq));
    }

    list.resize(new_list.size());
    copy(new_list.begin(), new_list.end(), list.begin());

    list = filter_seq(list, mass_spectrum, mass_tbl);
  }

  // Transform indecies in masses and filtering duplicates (due to repeating masses in spectrum)
  set<vector<size_t>> filtered_mass_list;
  for (const auto& seq : list)
  {
    vector<size_t> mass_seq;
    for (const auto idx : seq)
      mass_seq.push_back(mass_tbl[idx]);
    filtered_mass_list.insert(move(mass_seq));
  }

  return vector<vector<size_t>>(filtered_mass_list.begin(), filtered_mass_list.end());
}