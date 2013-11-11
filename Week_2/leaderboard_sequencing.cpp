#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

extern map<char, size_t> mass_tbl;
extern map<size_t, char> peptide_tbl;

vector<size_t> generate_circular_spectrum(const string& peptide);

static vector<vector<size_t>> expand(const vector<vector<size_t>>& start_seq)
{
  vector<vector<size_t>> ret;

  for (const auto& peptide : start_seq)
  {
    for (const auto mass : mass_tbl)
    {
      vector<size_t> new_seq(peptide);
      new_seq.push_back(mass.second);
      ret.push_back(move(new_seq));
    }
  }

  return ret;
}

static size_t score(const vector<size_t>& peptide,const vector<size_t>& model)
{
  size_t ret = 0;

  // Turn masses into specturm
  string peptide_str;
  for (const auto mass : peptide)
    peptide_str += peptide_tbl[mass];

  vector<size_t> spec = generate_circular_spectrum(peptide_str);

  for (const auto mass : spec)
  {
    if (binary_search(model.begin(), model.end(), mass))
      ret++;
  }
  return ret;
}


static size_t filter(vector<vector<size_t>>& board, const vector<size_t>& mass_spectrum, size_t N, const size_t highest_score)
{
  vector<vector<size_t>> ret;
  vector<size_t> score_board;

  // Compute score for every peptide in board
  for (const auto& peptide : board)
    score_board.push_back(score(peptide, mass_spectrum));

  // Cut of N scored items
  vector<size_t> sorted_scores(score_board);
  sort(sorted_scores.begin(), sorted_scores.end());

  set<size_t> N_max_scores; size_t idx = 0;
  while (N_max_scores.size() != N && idx < sorted_scores.size())
    N_max_scores.insert(sorted_scores[idx++]);

  // Filtering board for every score in N_max_scores, that greater than highest_score

  vector<size_t> N_max_scores2(N_max_scores.begin(), N_max_scores.end());
  sort(N_max_scores2.rbegin(), N_max_scores2.rend());

  size_t new_highest_score = highest_score;
  for (const auto score : N_max_scores2)
  {
    if (score > highest_score)
    {
      if (score > new_highest_score)
        new_highest_score = score;

      for (size_t i = 0; i < score_board.size(); ++i)
      {
        if (score_board[i] == score)
          ret.push_back(board[i]);

        if (ret.size() == N)
          goto M;
      }
    }
  }

  M:

  board.resize(ret.size());
  copy(ret.begin(), ret.end(), board.begin());

  return new_highest_score;
}

vector<vector<size_t>> leaderboard_sequencing(vector<size_t>& mass_spectrum, const size_t N)
{
  vector<vector<size_t>> leaderboard;
  
  vector<vector<size_t>> next_iter;
  for (const auto mass : mass_tbl)
    next_iter.push_back({ mass.second });

  size_t highest_score = 0;

  sort(mass_spectrum.begin(), mass_spectrum.end());

  while (highest_score <= mass_spectrum.size() && !next_iter.empty())
  {
    leaderboard.resize(next_iter.size());
    copy(next_iter.begin(), next_iter.end(), leaderboard.begin());

    next_iter = expand(leaderboard);
    highest_score = filter(next_iter, mass_spectrum, N, highest_score);
  }

  return leaderboard;
}