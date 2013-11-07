#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

// Pattern generator
map<char, char> alphabeth{ { 'A', 'T' }, { 'T', 'C' }, { 'C', 'G' }, { 'G', 'A' } };

static string next_pattern(const string& prev_pattern)
{
  string ret(prev_pattern);
  for (size_t i = prev_pattern.length() - 1; i >= 0; --i)
  {
    if (ret[i] != 'G')
    {
      ret[i] = alphabeth[ret[i]];
      break;
    }
    else
      ret[i] = alphabeth[ret[i]];
  }
  return ret;
}

static size_t max_num_misses(const string& input, const string& original)
{
  assert(input.length() == original.length());

  size_t num = 0;
  for (size_t i = 0; i < input.length(); i++)
  {
    if (input[i] != original[i])
      num++;
  }

  return num;
}

vector<string> approx_most_freq_substr(const string& input_str, size_t k, size_t d, size_t num_threads)
{
  assert(num_threads <= thread::hardware_concurrency());
  assert(num_threads == 1 || num_threads == 2 || num_threads == 4);

  vector<string> tbl;

  for (size_t i = 0; i <= input_str.size() - k; i++)
  {
    string substr = input_str.substr(i, k);
    tbl.push_back(substr);
  }

  mutex m;
  atomic<size_t> max_score(0);
  vector<string> ret;

  auto worker = [&](const string start_pattern)
  {
    string pattern = start_pattern;
    for (size_t i = 0; i < pow(4, k) / num_threads - 1; i++)
    {
      size_t score = 0;
      for (const auto& entry : tbl)
      {
        if (max_num_misses(pattern, entry) <= d)
          score++;
      }

      if (score > 0)
      {
        if (score >= max_score.load())
          if (score == max_score.load())
          {
            lock_guard<mutex> lg(m);
            ret.push_back(pattern);
          }
          else
          {
            max_score.store(score);
            lock_guard<mutex> lg(m);
            ret.clear();
            ret.push_back(pattern);
          }
      }
      pattern = next_pattern(pattern);
    }
  };

  string start_pattern(k, 'A');
  vector<thread> workers;

  for (size_t i = 0; i < num_threads; i++)
  {
    workers.push_back(thread(worker, start_pattern));
    start_pattern[0] = alphabeth[start_pattern[0]];
  }
  
  for (auto& thread_item : workers)
    thread_item.join();

  return ret;
}