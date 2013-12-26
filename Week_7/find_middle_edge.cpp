#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

vector<string> split_string(const string& input);

static void gen_matrix(map<string, int>& mat)
{
  const string file_name = "blosum.dat";

  vector<string> alphabeth = { "A", "C", "D", "E", "F", "G", "H", "I", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "V", "W", "Y" };
  ifstream f(file_name);
  string temp;

  getline(f, temp); // Ignore first string
  while (getline(f, temp))
  {
    auto buff(split_string(temp));
    auto prefix = buff[0];
    size_t i = 1;
    for each (const auto& var in alphabeth)
    {
      mat[prefix + var] = stoi(buff[i++]);
    }
  }
}

vector<int> get_middle_column(const string& s1, const string& s2, map<string, int>& score_mat, const size_t middle_point)
{
  const size_t n = s1.length() + 1;
  const size_t m = s2.length() + 1;

  const int del_penalty = -5;
  const int ins_penalty = -5;

  vector<int> prev_column(m);
  for (size_t i = 1; i < m; i++) prev_column[i] = prev_column[i - 1] + ins_penalty;

  vector<int> curr_column(m);

  vector<int> buff(3);
  // For every column
  for (size_t i = 1; i < middle_point + 1; i++)
  {
    // For every row
    for (size_t j = 0; j < m; j++)
    {
      // First row
      if (j == 0)
      {
        curr_column[j] = prev_column[j] + del_penalty;
        continue;
      }
      // Insert edge
      buff[0] = curr_column[j - 1] + ins_penalty;
      // Delete edge
      buff[1] = prev_column[j] + del_penalty;
      // Match / Mismatch edge
      string key{ s1[i - 1], s2[j - 1] };
      buff[2] = prev_column[j - 1] + score_mat[key];

      // Max from above
      curr_column[j] = *max_element(buff.begin(), buff.end());
    }
    prev_column.swap(curr_column);
  }

  return prev_column;
}

pair<pair<size_t, size_t>, pair<size_t, size_t>> find_middle_edge(const string& s1, const string& s2, map<string, int> score_mat)
{
  size_t middle_point = (s1.length() + 1) / 2;

  auto forward_column = get_middle_column(s1, s2, score_mat, middle_point - 1);

  string s1_reverse(s1.rbegin(), s1.rend()), s2_reverse(s2.rbegin(), s2.rend());
  auto backward_column = get_middle_column(s1_reverse, s2_reverse, score_mat, middle_point);

  vector<int> middle_column(s2.length() + 1);
  for (size_t i = 0; i < middle_column.size(); i++) middle_column[i] = forward_column[i] + backward_column[backward_column.size() - 1 - i];

  auto middle_node = max_element(middle_column.begin(), middle_column.end());
  size_t min_idx = distance(middle_column.begin(), middle_node);

  return make_pair(make_pair(min_idx, middle_point - 1), make_pair(min_idx + 1, middle_point));
}

pair<pair<size_t, size_t>, pair<size_t, size_t>> find_middle_edge(const string& s1, const string& s2)
{
  map<string, int> score_mat;
  gen_matrix(score_mat);

  return find_middle_edge(s1, s2, score_mat);
}

