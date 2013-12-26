#include <string>
#include <tuple>
#include <map>
#include <vector>
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

static vector<int> get_middle_column(const vector<int> init_val, const string& s1, const string& s2, map<string, int>& score_mat, const size_t middle_point)
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

static tuple<size_t, size_t, vector<int>> find_middle_node(const vector<int> left_init_val, const vector<int> rigth_init_val, const string& s1, const string& s2, map<string, int>& score_mat)
{
  size_t middle_point = s1.length() / 2;

  auto forward_column = get_middle_column(left_init_val, s1, s2, score_mat, middle_point);

  string s1_reverse(s1.rbegin(), s1.rend()), s2_reverse(s2.rbegin(), s2.rend());
  auto backward_column = get_middle_column(rigth_init_val, s1_reverse, s2_reverse, score_mat, s1.length() - middle_point);

  vector<int> middle_column(s2.length() + 1);
  for (size_t i = 0; i < middle_column.size(); i++) middle_column[i] = forward_column[i] + backward_column[backward_column.size() - 1 - i];

  auto middle_node = max_element(middle_column.begin(), middle_column.end());
  size_t min_idx = distance(middle_column.begin(), middle_node);

  return make_tuple(middle_point, min_idx, middle_column);
}

tuple<int, string, string> linear_space_alignment(const string& s1, const string& s2)
{
  map<string, int> score_mat;
  gen_matrix(score_mat);

  vector<int> init_column(s2.length() + 1);
  for (size_t i = 1; i < init_column.size(); i++) { init_column[i] = init_column[i - 1] - 5; }

  vector<int> init_column2(init_column);

  auto point = find_middle_node(init_column, init_column, s1, s2, score_mat);

  string s3 = "ANTLY";
  string s4 = "ANLY";

  auto& init_ref = get<2>(point);
  init_column.resize(init_column.size() - get<1>(point));
  auto point2 = find_middle_node(vector<int>(init_ref.begin() + get<1>(point), init_ref.end()), init_column, s3, s4, score_mat);

  string s5 = "TLY";
  string s6 = "LY";

  auto& init_ref2 = get<2>(point2);
  init_column.resize(init_column.size() - get<1>(point2));
  auto point3 = find_middle_node(vector<int>(init_ref2.begin() + get<1>(point2), init_ref2.end()), init_column, s5, s6, score_mat);

  string s7 = "LY";
  string s8 = "LY";
  auto& init_ref3 = get<2>(point3);
  init_column.resize(init_column.size() - get<1>(point3));
  auto point4 = find_middle_node(vector<int>(init_ref3.begin() + get<1>(point3), init_ref3.end()), init_column, s7, s8, score_mat);

  string s9 = "AN";
  string s10 = "AN";
  auto& init_ref_left = get<2>(point);
  auto& init_ref_right = get<2>(point2);

  vector<int> init_column_left(init_ref_left.begin() + get<1>(point), init_ref_left.end() - get<1>(point2));
  vector<int> init_column_right(init_ref_right.begin(), init_ref_right.begin() + get<1>(point2) + 1);

  auto point5 = find_middle_node(init_column_left, init_column_right, s9, s10, score_mat);

  return make_tuple(0, "", "");
}