#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> gibbs_sampler(const vector<string>& dna, const size_t k, const size_t t, const size_t N);

int main()
{
  size_t k, t, N;
  //cin >> k >> t >> N;
  //cin.ignore();
  k = 8, t = 5, N = 100;

  string input_str;
  vector<string> dna
  {
    "CGCCCCTCTCGGGGGTGTTCAGTAAACGGCCA",
    "GGGCGAGGTATGTGTAAGTGCCAAGGTGCCAG",
    "TAGTACCGAGACCGAAAGAAGTATACAGGCGT",
    "TAGATCAAGTTTCAGGTGCACGTCGGTGAACC",
    "AATCCACCAGCTCCACGTGCAATGTTGGCCTA"
  };
  
  //while(cin >> input_str)
  //  dna.push_back(input_str);

  auto ret = gibbs_sampler(dna, k, t, N);

  for (const auto& item : ret)
    cout << item << endl;

  cin.ignore();

  return 0;
}