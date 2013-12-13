#include <vector>
#include <string>
#include <map>

using namespace std;

static map<int, size_t> memo;

static size_t recursive_change(const int money, const vector<size_t>& coins)
{
  if (money == 0)
    return 0;

  size_t min_num = numeric_limits<size_t>::max();

  for (const auto coin : coins)
  {
    if (money >= coin)
    {
      size_t num_coins;
      if (memo.find(money - coin) == memo.end())
      {
        num_coins = recursive_change(money - coin, coins);
        memo[money - coin] = num_coins;
      }
      else
        num_coins = memo[money - coin];

      if (num_coins + 1 < min_num)
        min_num = num_coins + 1;
    }
  }

  return min_num;
}

size_t get_change(const size_t money, const vector<size_t>& coins)
{
  auto ret = recursive_change(money, coins);
  return ret;
}