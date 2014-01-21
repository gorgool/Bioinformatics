#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

static size_t get_idx(const vector<pair<char, size_t>>& text, const char letter, const size_t pos)
{
  size_t ret = 0;
  while (true)
  {
    if (text[ret].first == letter && text[ret].second == pos)
      return ret;
    ret++;
  }
}

static vector<pair<char, size_t>> to_column(const string& text)
{
  vector<pair<char, size_t>> ret;
  map<char, size_t> idx_tbl;
  for (const auto letter : text)
  {
    ret.push_back(make_pair(letter, idx_tbl[letter]));
    idx_tbl[letter]++;
  }
  return ret;
}

string BWT_to_string(const string& bwt)
{
  vector<pair<char, size_t>> last_column = to_column(bwt);
  string head = bwt;
  stable_sort(head.begin(), head.end());
  vector<pair<char, size_t>> first_column = to_column(head);

  string ret; bool reverse = true;
  pair<char, size_t>& temp = last_column[0];
  for (size_t idx = 0; idx < bwt.length(); idx++)
  {
    ret += temp.first;

    size_t pos = get_idx(first_column, temp.first, temp.second);
    temp = last_column[pos];
  }

  string temp_ret = string(ret.rbegin(), ret.rend());
  rotate(temp_ret.begin(), temp_ret.begin() + 1, temp_ret.end());
  return temp_ret;
}