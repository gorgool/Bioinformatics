#include <string>
#include <vector>
#include <algorithm>

using namespace std;

static bool string_cmp(const string& text, const size_t lhs, const size_t rhs)
{
  size_t left_idx = lhs, right_idx = rhs;
  for (size_t i = 0; i < text.length(); i++)
  {
    if (text[left_idx] == text[right_idx])
    {
      left_idx = (left_idx + 1) % text.length();
      right_idx = (right_idx + 1) % text.length();
      continue;
    } 
    else
    {
      if (text[left_idx] < text[right_idx])
        return true;
      else
        return false;
    }
  }
  return true;
}

string BWT(const string& text)
{
  vector<size_t> suffixes(text.length());

  size_t from = 0;
  generate_n(suffixes.begin(), text.length(), [&from](){ return from++; });

  sort(suffixes.begin(), suffixes.end(), [&text](const size_t lid, const size_t rid)
  {
    return string_cmp(text, lid, rid);
  });

  string ret;

  for (const auto& idx : suffixes)
  {
    if (idx == 0)
      ret += '$';
    else
      ret += text[idx - 1];
  }

  return ret;
}