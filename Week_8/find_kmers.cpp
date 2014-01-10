#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <map>

using namespace std;

static map<char, char> pairs =
{
  make_pair('A', 'T'),
  make_pair('T', 'A'),
  make_pair('C', 'G'),
  make_pair('G', 'C')
};

static string reverse_comp(const string& s)
{
  string ret(s);
  for (size_t i = 0; i < s.length(); i++)
  {
    ret[i] = pairs[s[i]];
  }
  return string(ret.rbegin(), ret.rend());
}

vector<pair<size_t, size_t>> find_kmers(const size_t k, const string& s1, const string& s2)
{
  size_t num_threads = thread::hardware_concurrency();
  vector<thread> thread_pool;
  vector<pair<size_t, size_t>> ret;
  mutex m;

  auto functor = [&](const size_t from_idx, const size_t to_idx)
  {
    for (size_t i = from_idx; i <= to_idx - k; i++)
    {
      auto substr = s1.substr(i, k);
      auto rev_substr = reverse_comp(substr);
      for (size_t j = 0; j <= s2.length() - k; j++)
      {
        auto test_substr = s2.substr(j, k);
        if (test_substr == substr || test_substr == rev_substr)
        {
          auto item = make_pair(i, j);
          lock_guard<mutex> lg(m);
          ret.push_back(item);
        }
      }
    }
  };

  size_t sector_len = s1.length() / num_threads;
  if (sector_len < k)
  {
    num_threads /= 2;
    sector_len = s1.length() / num_threads;
  }
  
  for (size_t i = 0; i < num_threads; i++)
  {
    if ((i + 1) == num_threads)
      thread_pool.push_back(thread(functor, sector_len * i, s1.length()));
    else
      thread_pool.push_back(thread(functor, sector_len * i, sector_len * (i + 1)));
  }

  for (auto& th : thread_pool)
    th.join();

  //sort(ret.begin(), ret.end(), [](const pair<size_t, size_t>& lhs, const pair<size_t, size_t>& rhs){ return lhs.second < rhs.second; });

  return ret;
}