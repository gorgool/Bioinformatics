#include <string>
#include <vector>

using namespace std;

vector<size_t> pattern_matching(const string& input_str, const string& sub_str)
{
	vector<size_t> ret;
	int pos = -1;

	while (true)
	{
		pos = input_str.find(sub_str, pos + 1);
		if (pos == string::npos)
			break;
		ret.push_back(pos);
	}

	return ret;
}