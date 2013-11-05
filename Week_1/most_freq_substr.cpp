#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

vector<string> most_freq_substr(const string& input_str, unsigned int k)
{
	map<string, unsigned int> tbl;

	for (size_t i = 0; i < input_str.size(); i++)
	{
		string substr = input_str.substr(i, k);

		auto entry = tbl.find(substr);
		if (entry != tbl.end())
		{
			tbl[entry->first] ++;
		}
		else
		{
			tbl.insert(make_pair(substr, 1));
		}
	}

	vector<unsigned int> temp;
	for (const auto& item : tbl)
	{
		temp.push_back(item.second);
	}

	sort(temp.begin(), temp.end());

	vector<string> ret;

	for (const auto& item : tbl)
	{
		if (item.second == temp[temp.size() - 1])
			ret.push_back(item.first);
	}

	return ret;
}