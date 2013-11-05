

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> most_freq_substr(const string& input_str, unsigned int k);

string reverse_complement(string& input_str);

vector<size_t> pattern_matching(const string& input_str, const string& sub_str);

vector<string> find_clumps(const string& input_str, size_t k, size_t L, size_t t);

int main()
{
	string input_str;
	cin >> input_str;

	size_t k, L, t;
	cin >> k >> L >> t;

	auto ret = find_clumps(input_str, k, L, t);

	for (const auto& item : ret)
	{
		cout << item << " ";
	}

	return 0;
}