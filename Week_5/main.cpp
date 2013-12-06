#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> contig_generation(const vector<string>& v);

int main()
{
  vector<string> input_text;
  string input_string;
  while (getline(cin, input_string))
  {
    input_text.push_back(input_string);
  }
 
  auto ret = contig_generation(input_text);
  for (const auto& item : ret)
    cout << item << endl;

  return 0;
}