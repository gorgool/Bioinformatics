#include <string>
#include <vector>
#include <algorithm>

using namespace std;

static string get_pattern(const string& text, const size_t offset)
{
  auto right = text.substr(offset);
  auto left = text.substr(0, offset);

  return right + left;
}

string BWT(const string& text)
{
  vector<string> patterns;
  for (size_t i = 0; i < text.length(); i++)
  {
    patterns.push_back(get_pattern(text, i));
  }

  sort(patterns.begin(), patterns.end());

  string ret;

  for (const auto& pattern : patterns)
  {
    ret += *pattern.rbegin();
  }

  return ret;
}