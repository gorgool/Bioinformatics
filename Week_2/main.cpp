#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <set>

using namespace std;

vector<set<size_t>> generate_cyclopeptide(vector<size_t>& mass_spectrum);

int main()
{
  string input_str;
  //cin >> input_str;
  input_str = "0 71 97 99 103 113 113 114 115 131 137 196 200 202 208 214 226 227 228 240 245 299 311 311 316 327 337 339 340 341 358 408 414 424 429 436 440 442 453 455 471 507 527 537 539 542 551 554 556 566 586 622 638 640 651 653 657 664 669 679 685 735 752 753 754 756 766 777 782 782 794 848 853 865 866 867 879 885 891 893 897 956 962 978 979 980 980 990 994 996 1022 1093";

  istringstream iss(input_str);
  vector<size_t> mass_spectrum(istream_iterator<size_t>{iss}, istream_iterator<size_t>());

  auto ret = generate_cyclopeptide(mass_spectrum);

  return 0;
}
