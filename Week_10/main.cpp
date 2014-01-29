#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<pair<size_t, size_t>> partial_suff_array(const string& text, const size_t k);

int main()
{
  string text;
  cin >> text;
  size_t k;
  cin >> k;

  auto ret = partial_suff_array(text, k);

  for (const auto var : ret)
    cout << var.first << "," << var.second << endl;

  return 0;
}