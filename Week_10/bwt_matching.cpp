#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>

using namespace std;

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

vector<size_t> BWT_matching(const string& bwt, const vector<string>& patterns)
{
  vector<size_t> ret(patterns.size());

  auto last_column = to_column(bwt);
  auto head = bwt;
  stable_sort(head.begin(), head.end());
  auto first_column = to_column(head);

  auto worker = [&](const vector<size_t>& ids)
  {
    for (const auto id : ids)
    {
      size_t idx = 0;
      vector<size_t> indices;

      auto pattern = patterns[id];

      for (size_t i = 0; i < bwt.length(); i++)
      {
        if (head[i] == pattern[pattern.size() - 1 - idx])
          indices.push_back(i);
      }

      idx++;

      while (idx != pattern.length())
      {
        // 1. filter
        auto last = remove_if(indices.begin(), indices.end(), [&](const size_t i){ return last_column[i].first != pattern[pattern.size() - 1 - idx]; });
        // 2. transform
        for (auto it = indices.begin(); it != last; it++)
        {
          auto letter = last_column[*it];
          *it = get_idx(first_column, letter.first, letter.second);
        }
        indices.resize(distance(indices.begin(), last));
        idx++;
      }

      ret[id] = indices.size();
    }
  };

  size_t num_threads = 4;
  vector<thread> thread_pool;
  if (patterns.size() <= num_threads)
  {
    for (size_t id = 0; id < patterns.size(); id++)
      thread_pool.push_back(thread(worker, vector<size_t>{ id }));
  }
  else
  {
    // Even
    if (patterns.size() % num_threads == 0)
    {
      vector<size_t> ids(patterns.size() / num_threads);
      for (size_t id = 0; id < num_threads; id++)
      {
        size_t counter = id * patterns.size() / num_threads;
        generate_n(ids.begin(), patterns.size() / num_threads, [&counter](){ return counter++; });
        thread_pool.push_back(thread(worker, ids));
      } 
    }
    //Odd
    else
    {
      vector<size_t> ids(patterns.size() / num_threads);
      for (size_t id = 0; id < num_threads - 1; id++)
      {
        size_t counter = id * patterns.size() / num_threads;
        generate_n(ids.begin(), patterns.size() / num_threads, [&counter](){ return counter++; });
        thread_pool.push_back(thread(worker, ids));
      }
      size_t counter = 3 * patterns.size() / num_threads;
      ids.resize(patterns.size() - counter);
      generate_n(ids.begin(), patterns.size() - counter, [&counter](){ return counter++; });
      thread_pool.push_back(thread(worker, ids));
    }
  }

  for (auto& th : thread_pool)
    th.join();

  return ret;
}