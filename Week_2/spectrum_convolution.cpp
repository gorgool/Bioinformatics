#include <vector>
#include <algorithm>

using namespace std;

vector<size_t> spectrum_convolution(vector<size_t>& mass_spectrum)
{
  vector<size_t> ret;
  ret.reserve(mass_spectrum.size() * mass_spectrum.size() / 2);

  for (const auto mass : mass_spectrum)
  {
    for (const auto entry : mass_spectrum)
    {
      int diff = entry - mass;
      if (diff > 0)
        ret.push_back(diff);
    }
  }

  return ret;
}