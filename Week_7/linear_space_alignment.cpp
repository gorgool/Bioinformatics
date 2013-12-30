#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

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

static vector<int> get_middle_column(const vector<int>& init_val, const string& s1, const string& s2, map<string, int>& score_mat, const size_t middle_point)
{
  const size_t n = s1.length() + 1;
  const size_t m = s2.length() + 1;

  const int del_penalty = -5;
  const int ins_penalty = -5;

  vector<int> prev_column = init_val;

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

static vector<pair<size_t, size_t>> path;

static void find_middle_node(
  const size_t left_i, 
  const size_t left_j, 
  const vector<int>& left_init_val, 
  const vector<int>& rigth_init_val, 
  const string& s1, const string& s2, 
  map<string, int>& score_mat)
{
  size_t middle_point = s1.length() / 2;

  vector<int> middle_column(s2.length() + 1);
  // Compute indeces of middle node and middle column
  {
    auto forward_column = get_middle_column(left_init_val, s1, s2, score_mat, middle_point);
    string s1_reverse(s1.rbegin(), s1.rend()), s2_reverse(s2.rbegin(), s2.rend());
    auto backward_column = get_middle_column(rigth_init_val, s1_reverse, s2_reverse, score_mat, s1.length() - middle_point);
    for (size_t i = 0; i < middle_column.size(); i++) middle_column[i] = forward_column[i] + backward_column[backward_column.size() - 1 - i];
  }
 
  size_t min_idx = distance(middle_column.begin(), max_element(middle_column.begin(), middle_column.end()));

  // Left path
  {
    auto left1 = vector<int>(left_init_val.begin(), left_init_val.begin() + min_idx + 1);
    auto right1 = vector<int>(middle_column.begin(), middle_column.begin() + min_idx + 1);
    right1 = vector<int>(right1.rbegin(), right1.rend());

    auto s1_left = s1.substr(0, middle_point);
    auto s2_left = s2.substr(0, min_idx);

    if (!(s1_left.empty() && s2_left.empty()))
      find_middle_node(left_i, left_j, left1, right1, s1_left, s2_left, score_mat);
  }

  pair<size_t, size_t> last;
  if (!path.empty())
  {
    last = path[path.size() - 1];
    if (!(last.first == left_i + middle_point && last.second == left_j + min_idx))
      path.push_back(make_pair(left_i + middle_point, left_j + min_idx));
  } 
  else
    path.push_back(make_pair(left_i + middle_point, left_j + min_idx));

  // Right path
  {
    auto left2 = vector<int>(middle_column.begin() + min_idx, middle_column.end());
    auto right2 = vector<int>(rigth_init_val.begin(), rigth_init_val.end() - min_idx);

    auto s1_right = s1.substr(middle_point);
    auto s2_right = s2.substr(min_idx);

    if (!(s1_right.empty() && s2_right.empty()) && !(s1_right == s1 && s2_right == s2))
      find_middle_node(left_i + middle_point, left_j + min_idx, left2, right2, s1_right, s2_right, score_mat);
  }

  return;
}

static int build_strings(vector<pair<size_t, size_t>>& path, map<string, int>& score_mat, string& ret1, string& ret2, const string& s1, const string& s2)
{
  size_t idx_s1 = 0, idx_s2 = 0;
  int score = 0, sigma = -5;
  for (size_t i = 1; i < path.size(); i++)
  {
    auto& prev_node = path[i - 1];

    // Deletion
    if (path[i].first == prev_node.first)
    {
      ret1 += '-';
      ret2 += s2[idx_s2++];
      score += sigma;
    }
    // Insertion
    else if (path[i].second == prev_node.second)
    {
      ret1 += s1[idx_s1++];
      ret2 += '-';
      score += sigma;
    }
    // Matches / Mismatches
    else
    {
      ret1 += s1[idx_s1];
      ret2 += s2[idx_s2];
      string key{ s1[idx_s1++], s2[idx_s2++] };
      score += score_mat[key];
    }
  }

  return score;
}

tuple<int, string, string> linear_space_alignment(const string& s1, const string& s2)
{
  map<string, int> score_mat;
  gen_matrix(score_mat);

  {
    vector<int> init_column(s2.length() + 1);
    for (size_t i = 1; i < init_column.size(); i++) { init_column[i] = init_column[i - 1] - 5; }

    find_middle_node(0, 0, init_column, init_column, s1, s2, score_mat);
    path.push_back(make_pair(s1.length(), s2.length()));
  }

  string res1, res2;
  int score = build_strings(path, score_mat, res1, res2, s1, s2);

  return make_tuple(score, res1, res2);
}