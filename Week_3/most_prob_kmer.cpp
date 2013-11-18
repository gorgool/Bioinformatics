#include <string>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

static double probability(const string& dna, const vector<map<char, double>>& profile)
{
  assert(dna.length() == profile.size());

  double ret = 1.0;
  for (size_t i = 0; i < dna.size(); ++i)
  {
    auto entry = profile[i];
    ret *= entry[dna[i]];
  }
  
  return ret;
}

string most_prob_kmer(const string& dna, const size_t k, const vector<map<char, double>>& profile)
{
  map<double, string> prob_tbl;

  for (size_t pos = 0; pos < dna.length() - k + 1; pos++)
  {
    string substr = dna.substr(pos, k);

    prob_tbl.insert({ probability(substr, profile), substr });
  }

  return prob_tbl.rbegin()->second;
}