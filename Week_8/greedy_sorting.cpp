#include <string>
#include <vector>
#include <algorithm>

using namespace std;

static string to_string_seq(const vector<int>& int_seq)
{
  string ret("(");
  for (size_t idx = 0; idx < int_seq.size(); ++idx)
  {
    if (int_seq[idx] > 0)
      ret += "+";
    ret += to_string(int_seq[idx]);
    if (idx != int_seq.size() - 1)
      ret += " ";
  }
  return ret + ")";
}

static void mutate_seq(vector<int>::iterator from, vector<int>::iterator to, vector<int>& seq)
{
  while (true)
  {
    if (from == to)
    {
      *from = -*from;
      break;
    }

    if (distance(from, to) == 1)
    {
      *from = -*from;
      *to = -*to;
      swap(*from, *to);
      break;
    }

    *from = -*from;
    *to = -*to;
    swap(*from, *to);
    to--, from++;
  }
}

vector<string> greedy_sorting(const vector<int> in_seq)
{
  vector<string> ret;
  string string_seq = to_string_seq(in_seq);
  vector<int> in = in_seq;
  int idx = 1;
  for (auto it = in.begin(); idx != in.size() + 1;)
  {
    // If match
    if (*it == idx)
    {
      it++;
      idx++;
      continue;
    }

    // If reverse match
    if (*it == -idx)
    {
      *it = -*it;
      it++;
      idx++;
      ret.push_back(to_string_seq(in));
      continue;
    }

    // If missmatch
    auto end_seq = find_if(it, in.end(), [idx](const int item) { return item == idx || item == -idx; });
    mutate_seq(it, end_seq, in);
    ret.push_back(to_string_seq(in));
  }

  return ret;
}