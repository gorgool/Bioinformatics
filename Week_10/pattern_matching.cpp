#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

vector<size_t> construct_suffix_array(const string& text); 
string BWT(const string& text);

static map<size_t, map<char, size_t>> checkpoints;
static size_t C = 100;

static size_t count(const char symbol, const size_t until, const string& text)
{
  size_t ret = 0;
  for (size_t i = 0; i < until; i++)
  {
    if (text[i] == symbol) ret++;
  }
  return ret;
}

static size_t speed_count(const char symbol, const size_t until, const string& text)
{
  if (until != 0 && until - 1 % C == 0)
    return checkpoints[until][symbol];

  size_t ret = 0, key = 0;
  if (until > C)
  {
    key = C * static_cast<size_t>((until - 1) / C);
    ret = checkpoints[key][symbol];
  }

  for (size_t i = key; i < until; i++)
  {
    if (text[i] == symbol) ret++;
  }

  return ret;
}

static size_t first_occurance(const char symbol, const string& text)
{
  for (size_t i = 0; i < text.length(); i++)
  {
    if (text[i] == symbol) return i;
  }
  return 0;
}

static bool check(const size_t top, const size_t bottom, const char symbol, const string& text)
{
  for (size_t idx = top; idx <= bottom; idx++)
  {
    if (text[idx] == symbol)
      return true;
  }
  return false;
}

vector<size_t> pattern_matching(const string& text, const vector<string>& patterns)
{
  auto bwt = BWT(text + "$");
  auto head = bwt;
  std::sort(head.begin(), head.end());

  vector<size_t> ret;
  auto suffix_array = construct_suffix_array(text + "$");

  map<char, size_t> occur_tbl
  {
    make_pair('A', first_occurance('A', head)),
    make_pair('T', first_occurance('T', head)),
    make_pair('C', first_occurance('C', head)),
    make_pair('G', first_occurance('G', head))
  };

  for (size_t break_point = 0; break_point < text.length(); break_point += C)
  {
    map<char, size_t> checkpoint
    {
      make_pair('A', count('A', break_point, bwt)),
      make_pair('T', count('T', break_point, bwt)),
      make_pair('C', count('C', break_point, bwt)),
      make_pair('G', count('G', break_point, bwt)),
    };

    checkpoints.insert(make_pair(break_point, checkpoint));
  }


  for (const auto& pattern : patterns)
  {
    size_t top = 0;
    size_t bottom = bwt.size() - 1;
    int idx = pattern.size() - 1;

    for (; idx >= 0; idx--)
    {
      if (top <= bottom)
      {
        char symbol = pattern[idx];
        if (check(top, bottom, symbol, bwt))
        {
          auto offset = occur_tbl[symbol];
          top = offset + speed_count(symbol, top, bwt);
          bottom = offset + speed_count(symbol, bottom + 1, bwt) - 1;
        }
        else
          break;
      }
      else
        break;
    }

    if (idx < 0)
    {
      for (size_t i = top; i <= bottom; i++)
      {
        ret.push_back(suffix_array[i]);
      }
    }
  }

  std::sort(ret.begin(), ret.end());

  return ret;
}