#include <string>
#include <map>

using namespace std;

map<char, char> pairs =
{
	make_pair('A', 'T'),
	make_pair('T', 'A'),
	make_pair('C', 'G'),
	make_pair('G', 'C')
};

string reverse_complement(const string& input_str)
{
  string ret(input_str);
	for (size_t i = 0; i < input_str.length(); i++)
	{
		ret[i] = pairs[input_str[i]];
	}
  return string(ret.rbegin(), ret.rend());
}