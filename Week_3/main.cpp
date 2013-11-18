#include <iostream>
#include <string>
#include <vector>

using namespace std;

string median_string(const vector<string>& dna, const size_t k);

int main()
{
  size_t k;
  cin >> k;
  cin.ignore();
  //k = 3;

  vector<string> dna;
  string input_str;
  while (cin >> input_str)
    dna.push_back(input_str);

  auto ret = median_string(dna, k);

  cout << ret;

  return 0;
}