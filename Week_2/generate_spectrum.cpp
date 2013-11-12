#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <cassert>
#include <algorithm>

using namespace std;

map<char, size_t> mass_tbl
{
  { 'G', 57 },
  { 'A', 71 },
  { 'S', 87 },
  { 'P', 97 },
  { 'V', 99 },
  { 'T', 101 },
  { 'C', 103 },
  { 'I', 113 },
  { 'L', 113 },
  { 'N', 114 },
  { 'D', 115 },
  { 'K', 128 },
  { 'Q', 128 },
  { 'E', 129 },
  { 'M', 131 },
  { 'H', 137 },
  { 'F', 147 },
  { 'R', 156 },
  { 'Y', 163 },
  { 'W', 186 }
};

static size_t compute_mass(const string& proteins)
{
  size_t total_mass = 0;
  for (auto protein : proteins)
  {
    total_mass += mass_tbl[protein];
  }

  return total_mass;
}

static string get_substr(const string& str, size_t from, size_t length)
{
  assert(str.length() >= length);

  string ret;

  if (from + length <= str.length())
    ret = str.substr(from, length);
  else
  {
    size_t diff = str.length() - from;
    ret = str.substr(from, diff) + str.substr(0, length - diff);;
  }

  return ret;
}

vector<size_t> generate_circular_spectrum(const string& peptide)
{
  vector<size_t> ret;
  ret.push_back(0);
  for (size_t i = 1; i < peptide.length(); i++)
  {
    for (size_t j = 0; j < peptide.length(); j++)
    {
      string substr = get_substr(peptide, j, i);
      ret.push_back(compute_mass(substr));
    }
  }
  ret.push_back(compute_mass(peptide));
  sort(ret.begin(), ret.end());
  return ret;
}

vector<size_t> generate_linear_spectrum(const string& peptide)
{
  vector<size_t> ret;
  ret.push_back(0);

  for (size_t i = 1; i < peptide.length(); i++)
  {
    for (size_t j = 0; j < peptide.length(); j++)
    {
      if (i + j <= peptide.length())
      {
        string substr = peptide.substr(j, i);
        ret.push_back(compute_mass(substr));
      }
    }
  }
  ret.push_back(compute_mass(peptide));

  return ret;
}

vector<size_t> generate_circular_spectrum(const vector<size_t>& peptide)
{
  vector<size_t> ret;
  ret.push_back(0);
  for (size_t i = 1; i < peptide.size(); i++)
  {
    for (size_t j = 0; j < peptide.size(); j++)
    {
      vector<size_t> range;
      if (i + j > peptide.size())
      {
        range = vector<size_t>(peptide.begin() + j, peptide.end());
        range.insert(range.end(), peptide.begin(), peptide.begin() + peptide.size() - i);
      }
      else
        range = vector<size_t>(peptide.begin() + j, peptide.begin() + j + i);

      ret.push_back(accumulate(range.begin(), range.end(), 0));
    }
  }
  ret.push_back(accumulate(peptide.begin(), peptide.end(), 0));
  sort(ret.begin(), ret.end());
  return ret;
}