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

static vector<pair<vector<size_t>, size_t>> expand(const vector<pair<vector<size_t>, size_t>>& start_seq)
{
  vector<pair<vector<size_t>, size_t>> ret;

  for (const auto& peptide : start_seq)
  {
    for (const auto mass : mass_list)
    {
      pair<vector<size_t>, size_t> new_seq(peptide);
      new_seq.first.push_back(mass);
      new_seq.second = 0;

      ret.push_back(move(new_seq));
    }
  }

  return ret;
}

static size_t score(const vector<size_t>& test_spectrum,const vector<size_t>& model_spectrum)
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

static void filter(vector<pair<vector<size_t>, size_t>>& board, const vector<size_t>& mass_spectrum, size_t N, const size_t highest_score)
{
  vector<pair<vector<size_t>, size_t>> ret;

  // Compute score for every peptide in board
  for (auto& peptide : board)
    peptide.second = score(generate_circular_spectrum(peptide.first), mass_spectrum);
    

  sort(board.begin(), board.end(), []
    (const pair<vector<size_t>, size_t>& lhs, const pair<vector<size_t>, size_t>& rhs)
  {
    return lhs.second > rhs.second;
  });


  // Simplify !!!
  set<size_t> scores;
  for (auto& peptide : board)
  {
    if (scores.size() == N)
      break;

    int mass = accumulate(peptide.first.begin(), peptide.first.end(), 0);

    if (mass > mass_spectrum[mass_spectrum.size() - 1])
      continue;

    if (!binary_search(mass_spectrum.begin(), mass_spectrum.end(), mass))
      continue;

    scores.insert(peptide.second);
    ret.push_back(move(peptide));
  }

  board.resize(ret.size());
  copy(ret.begin(), ret.end(), board.begin());

  return;
}

static size_t get_leaders(const vector<pair<vector<size_t>, size_t>>& board, vector<pair<vector<size_t>, size_t>>& leaders, const size_t highest_score)
{
  size_t best_score = board[0].second;

  //if (best_score < highest_score)
  //  return highest_score;

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
    highest_score = get_leaders(next_iter, leaderboard, highest_score);
    next_iter = expand(next_iter);
    filter(next_iter, mass_spectrum, N, highest_score);
  }

  

  return leaderboard;
}