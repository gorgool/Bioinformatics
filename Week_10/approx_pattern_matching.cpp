#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>

using namespace std;

string BWT(const string& text);
vector<size_t> construct_suffix_array(const string& text);

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
  if (until != 0 && (until) % C == 0)
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
  for (int idx = top; idx <= bottom; idx++)
  {
    if (text[idx] == symbol)
      return true;
  }
  return false;
}

vector<string> to_seeds(const string& pattern, const size_t k)
{
  size_t len = pattern.length() / (k + 1);
  vector<string> ret;
  size_t idx = 0; size_t count = 0;
  while (count <= k)
  {
    // If last
    if (count == k)
      ret.push_back(pattern.substr(idx));
    else
      ret.push_back(pattern.substr(idx, len));

    count++;
    idx += len;
  }
  return ret;
}

static size_t max_num_misses(const string& input, const string& original)
{
  size_t num = 0;
  for (size_t i = 0; i < input.length(); i++)
  {
    if (input[i] != original[i])
      num++;
  }

  return num;
}

vector<size_t> approx_pattern_matching(const string& text, const vector<string>& patterns, const size_t k)
{
  vector<size_t> ret;
  
  auto bwt = BWT(text + "$");
  auto head = bwt;
  std::sort(head.begin(), head.end());

  auto suffix_array = construct_suffix_array(text + "$");

  map<char, size_t> occur_tbl
  {
    make_pair('A', first_occurance('A', head)),
    make_pair('T', first_occurance('T', head)),
    make_pair('C', first_occurance('C', head)),
    make_pair('G', first_occurance('G', head))
  };

  for (size_t break_point = 0; break_point <= text.length(); break_point += C)
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
    auto seeds = to_seeds(pattern, k);
    size_t offset = 0;
    set<size_t> positions;
    for (const auto seed : seeds)
    {
      size_t top = 0;
      size_t bottom = bwt.size() - 1;
      int idx = seed.size() - 1;

      for (; idx >= 0; idx--)
      {
        if (top <= bottom)
        {
          char symbol = seed[idx];
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

      // If find
      if (idx < 0)
      {
        for (size_t i = top; i <= bottom; i++)
        {
          int pos = suffix_array[i] - offset;

          if (pos < 0 || (pos + pattern.length()) > text.length())
            continue;

          auto substr = text.substr(pos, pattern.length());

          if (max_num_misses(substr, pattern) <= k && substr.length() == pattern.length())
            positions.insert(pos);
        }
      }
      offset += seed.length();
    }
    ret.insert(ret.end(), positions.begin(), positions.end());
  }

  sort(ret.begin(), ret.end());

  return ret;
}