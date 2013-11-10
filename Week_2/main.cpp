#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <set>

using namespace std;

vector<vector<size_t>> generate_cyclopeptide(vector<size_t>& mass_spectrum);

int main()
{
  string input_str;
  //getline(cin, input_str);
  input_str = "0 71 87 101 101 103 114 115 128 128 156 172 199 201 202 204 215 229 243 259 284 286 300 300 302 316 330 332 360 373 387 399 401 403 414 417 444 486 488 488 488 501 502 504 515 545 559 589 600 602 603 616 616 616 618 660 687 690 701 703 705 717 731 744 772 774 788 802 804 804 818 820 845 861 875 889 900 902 903 905 932 948 976 976 989 990 1001 1003 1003 1017 1033 1104";

  istringstream iss(input_str);
  vector<size_t> mass_spectrum(istream_iterator<size_t>{iss}, istream_iterator<size_t>());

  auto ret = generate_cyclopeptide(mass_spectrum);

  for (const auto& seq : ret)
  {
    for (size_t i = 0; i < seq.size(); ++i)
    {
      cout << seq[i];
      if (i < seq.size() - 1)
        cout << "-";
    }
    cout << " ";
  }

  cin.ignore();

  return 0;
}
