#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <atomic>

using namespace std;

static size_t count(const char symbol, const size_t until, const string& text)
{
  size_t ret = 0;
  for (size_t i = 0; i < until; i++)
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
  for (size_t idx = bottom; idx >= top; idx--)
  {
    if (text[idx] == symbol)
      return true;
  }
  return false;
}

vector<size_t> BWT_matching_ench(const string& bwt, const vector<string>& patterns)
{
  vector<size_t> ret(patterns.size());
  auto head = bwt;
  stable_sort(head.begin(), head.end());

  auto worker = [&](const vector<size_t>& ids)
  {
    for (const auto id : ids)
    {
      size_t top = 0;
      size_t bottom = bwt.size() - 1;
      auto pattern = patterns[id];

      int idx = pattern.size() - 1;

      while (top <= bottom)
      {
        if (idx < 0)
        {
          ret[id] = bottom - top + 1;
          break;
        } 
        char symbol = pattern[idx];
        if (check(top, bottom, symbol, bwt))
        {
          top = first_occurance(symbol, head) + count(symbol, top, bwt);
          bottom = first_occurance(symbol, head) + count(symbol, bottom + 1, bwt) - 1;
        }
        else
        {
          ret[id] = 0;
          break;
        }
        idx--;
      }     
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