#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <numeric>

using namespace std;

extern map<char, size_t> mass_tbl;
vector<size_t> generate_circular_spectrum(const vector<size_t>& peptide);

static vector<pair<vector<size_t>, size_t>> expand(const vector<pair<vector<size_t>, size_t>>& start_seq)
{
  vector<pair<vector<size_t>, size_t>> ret;

  for (const auto& peptide : start_seq)
  {
    for (const auto mass : mass_tbl)
    {
      pair<vector<size_t>, size_t> new_seq(peptide);
      new_seq.first.push_back(mass.second);
      new_seq.second = 0;

      ret.push_back(move(new_seq));
    }
  }

  return ret;
}

static size_t score(const vector<size_t>& peptide,const vector<size_t>& model)
{
  size_t ret = 0;

  vector<size_t> spec = generate_circular_spectrum(peptide);

  for (const auto mass : spec)
  {
    if (binary_search(model.begin(), model.end(), mass))
      ret++;
  }
  return ret;
}

static void filter(vector<pair<vector<size_t>, size_t>>& board, const vector<size_t>& mass_spectrum, size_t N, const size_t highest_score)
{
  vector<pair<vector<size_t>, size_t>> ret;

  // Compute score for every peptide in board
  for (auto& peptide : board)
    peptide.second = score(peptide.first, mass_spectrum);

  sort(board.begin(), board.end(), []
    (const pair<vector<size_t>, size_t>& lhs, const pair<vector<size_t>, size_t>& rhs)
  {
    return lhs.second > rhs.second;
  });

  set<size_t> scores;
  for (auto& peptide : board)
  {
    if (scores.size() == N)
      break;

    if (accumulate(peptide.first.begin(), peptide.first.end(), 0) > mass_spectrum[mass_spectrum.size() - 1])
      continue;

    scores.insert(peptide.second);
    ret.push_back(move(peptide));
  }

  board.resize(ret.size());
  copy(ret.begin(), ret.end(), board.begin());

  return;
}

static void get_leaders(const vector<pair<vector<size_t>, size_t>>& board, vector<pair<vector<size_t>, size_t>>& leaders)
{
  size_t best_score = board[0].second;
  size_t last_score = best_score, idx = 0;
  auto it = board.begin();

  do
  {
    idx++; it++;
    if (idx >= board.size())
      break;
    last_score = board[idx].second;
  } while (best_score == last_score);
  
  leaders.resize(idx);
  copy(board.begin(), it, leaders.begin());

  return;
}

vector<pair<vector<size_t>, size_t>> leaderboard_sequencing(vector<size_t>& mass_spectrum, const size_t N)
{
  vector<pair<vector<size_t>, size_t>> leaderboard;
  
  vector<pair<vector<size_t>, size_t>> next_iter;
  for (const auto mass : mass_tbl)
    next_iter.push_back({ { mass.second }, 0 });

  size_t highest_score = 0;

  sort(mass_spectrum.begin(), mass_spectrum.end());

  while (!next_iter.empty())
  {
    // Copy highest score item in leaderborad
    get_leaders(next_iter, leaderboard);

    next_iter = expand(next_iter);
    filter(next_iter, mass_spectrum, N, highest_score);
  }

  return leaderboard;
}