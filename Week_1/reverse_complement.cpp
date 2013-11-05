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

string reverse_complement(string& input_str)
{
	for (size_t i = 0; i < input_str.length(); i++)
	{
		input_str[i] = pairs[input_str[i]];
	}
	return string(input_str.rbegin(), input_str.rend());
}