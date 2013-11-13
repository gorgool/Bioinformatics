#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

vector<size_t> mass_list{ 57, 71, 87, 97, 99, 101, 103, 113, 114, 115, 128, 129, 131, 137, 147, 156, 163, 186 };
vector<size_t> generate_circular_spectrum(const vector<size_t>& peptide);

 static size_t score(const vector<size_t>& test_spectrum, const vector<size_t>& model_spectrum)
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
      new_seq.second = score(generate_circular_spectrum(new_seq.first), mass_spectrum);
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

vector<size_t> leaderboard_sequencing(vector<size_t>& mass_spectrum, const size_t N)
{
  vector<pair<vector<size_t>, size_t>> leaderboard;
  pair<vector<size_t>, size_t> leader_peptide;
  
  for (const auto mass : mass_list)
    leaderboard.push_back({ vector<size_t>{mass} , 0 });

  sort(mass_spectrum.begin(), mass_spectrum.end());

  while (!leaderboard.empty())
  {
    leaderboard = expand(leaderboard, mass_spectrum);
    vector<pair<vector<size_t>, size_t>> new_leaderboard;

    for (auto& peptide : leaderboard)
    {
      size_t mass = accumulate(peptide.first.begin(), peptide.first.end(), 0);
      if (mass == mass_spectrum[mass_spectrum.size() - 1])
      {
        if (peptide.second > leader_peptide.second)
        {
          leader_peptide.first.clear();
          leader_peptide = peptide;
        }
      }
      else if (mass > mass_spectrum[mass_spectrum.size() - 1])
        continue;

      new_leaderboard.push_back(peptide);
    } 
    filter(new_leaderboard, mass_spectrum, N);

    leaderboard.resize(new_leaderboard.size());
    copy(new_leaderboard.begin(), new_leaderboard.end(), leaderboard.begin());
  }

  return leader_peptide.first;
}