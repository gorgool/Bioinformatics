#include <string>
#include <vector>
#include <numeric>
#include <map>

using namespace std;

vector<string> find_clumps(const string& input_str, size_t k, size_t L, size_t t)
{
	map<string, vector<size_t>> tbl;

	for (size_t i = 0; i < input_str.size(); i++)
	{
		string substr = input_str.substr(i, k);

		auto entry = tbl.find(substr);

		// If found
		if (entry != tbl.end())
		{
			tbl[entry->first].push_back(i);
		}
		else
		{
			tbl.insert(make_pair(substr, vector<size_t>{i}));
		}
	}

	// Filter by t
	map<string, vector<size_t>> filtered_tbl;

	for (const auto& item : tbl)
	{
		if (item.second.size() >= t)
			filtered_tbl.insert(item);
	}


	vector<string> ret;
	// Filter by L
	for (const auto& entry : filtered_tbl)
	{
		vector<size_t> window;
		for (auto item : entry.second)
		{
			window.push_back(item);

			if ((window[window.size() - 1] - window[0]) > L)
			{
				window = vector<size_t>(window.begin() + 1, window.end());
			}

			if (window.size() == t && (window[window.size() - 1] - window[0]) <= L)
			{
				ret.push_back(entry.first);
				break;
			}
		}
	}

	return ret;
}