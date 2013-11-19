#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> greedy_motif_search(const vector<string>& dna, const size_t k, const size_t t);

int main()
{
  size_t k, t;
  cin >> k >> t;
  cin.ignore();
  //k = 3, t = 5;

  string input_str;
  vector<string> dna;
  
  while(cin >> input_str)
    dna.push_back(input_str);

  auto ret = greedy_motif_search(dna, k, t);

  for (const auto& item : ret)
    cout << item << endl;

  return 0;
}