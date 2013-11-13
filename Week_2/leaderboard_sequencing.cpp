#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <numeric>
#include <iostream>

using namespace std;

vector<size_t> mass_list{ 57, 71, 87, 97, 99, 101, 103, 113, 114, 115, 128, 129, 131, 137, 147, 156, 163, 186 };
vector<size_t> generate_circular_spectrum(const vector<size_t>& peptide);

 size_t score(const vector<size_t>& test_spectrum, const vector<size_t>& model_spectrum)
{
  int m_idx = 0, t_idx = 0;

  auto next_idx = [&](int& m_idx, int& t_idx)
  {
    if (test_spectrum[t_idx] < model_spectrum[m_idx])
    {
      if (t_idx < test_spectrum.size() - 1)
        t_idx++;
      else
        t_idx = -1;
    }
    else
    {
      if (m_idx < model_spectrum.size() - 1)
        m_idx++;
      else
        m_idx = -1;
    }
  };

  size_t score = 0;

  while (m_idx != -1 && t_idx != -1)
  {
    if (test_spectrum[t_idx] == model_spectrum[m_idx])
    {
      score++;

      if (t_idx < test_spectrum.size() - 1)
        t_idx++;
      else
        break;

      if (m_idx < model_spectrum.size() - 1)
        m_idx++;
      else
        break;

      continue;
    }

    next_idx(m_idx, t_idx);
  }

  return score;
}

static vector<pair<vector<size_t>, size_t>> expand(const vector<pair<vector<size_t>, size_t>>& start_seq, const vector<size_t>& mass_spectrum)
{
  vector<pair<vector<size_t>, size_t>> ret;

  for (const auto& peptide : start_seq)
  {
    for (const auto mass : mass_list)
    {
      pair<vector<size_t>, size_t> new_seq(peptide);
      new_seq.first.push_back(mass);

      vector<size_t> spectrum = generate_circular_spectrum(new_seq.first);

      if (spectrum[spectrum.size() - 1] > mass_spectrum[mass_spectrum.size() - 1])
        continue;

      new_seq.second = score(spectrum, mass_spectrum);

      ret.push_back(move(new_seq));
    }
  }

  return ret;
}

static void filter(vector<pair<vector<size_t>, size_t>>& board, const vector<size_t>& mass_spectrum, size_t N)
{
  vector<pair<vector<size_t>, size_t>> ret;

  sort(board.begin(), board.end(), []
    (const pair<vector<size_t>, size_t>& lhs, const pair<vector<size_t>, size_t>& rhs)
  {
    return lhs.second > rhs.second;
  });

  if (board.size() < N)
    return;
  else
    board.resize(N);

  return;
}

static size_t get_leaders(const vector<pair<vector<size_t>, size_t>>& board, vector<pair<vector<size_t>, size_t>>& leaders)
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

  return best_score;
}

vector<pair<vector<size_t>, size_t>> leaderboard_sequencing(vector<size_t>& mass_spectrum, const size_t N)
{
  vector<pair<vector<size_t>, size_t>> leaderboard;
  
  vector<pair<vector<size_t>, size_t>> next_iter;
  for (const auto mass : mass_list)
    next_iter.push_back({ { mass }, 0 });

  size_t highest_score = 0;

  sort(mass_spectrum.begin(), mass_spectrum.end());

  while (!next_iter.empty())
  {
    // Copy highest score item in leaderborad
    get_leaders(next_iter, leaderboard);
    next_iter = expand(next_iter, mass_spectrum);
    cout << next_iter.size() << endl;
    filter(next_iter, mass_spectrum, N);
  }

  return leaderboard;
}