#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> split_string(const string& input);
vector<string> greedy_sorting(const vector<int> in_seq);

int main()
{
  string input;
  getline(cin, input);

  vector<int> in_seq;
  auto v = split_string(input);
  for (const auto var : v) in_seq.push_back(stoi(var));

  auto ret = greedy_sorting(in_seq);

  for (const auto& item : ret)
    cout << item << endl;

  return 0;
}