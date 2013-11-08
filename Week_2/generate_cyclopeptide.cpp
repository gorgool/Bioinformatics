#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<size_t>> generate_cyclopeptide(vector<size_t> mass_spectrum)
{
  vector<size_t> spec_perm(mass_spectrum);

  vector<vector<size_t>> ret;

  do 
  {
    ret.push_back(spec_perm);
  } while (next_permutation(spec_perm.begin(), spec_perm.end()));
  
  return ret;
}