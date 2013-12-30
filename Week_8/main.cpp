#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <sstream>

using namespace std;

template<typename T, char ... delims>
vector<T> split_string(const string& input)
{
  static char _delims[] = {delims ...};
  struct tokens : ctype<char>
  {
    tokens() : ctype<char>(get_table()) {}

    static ctype_base::mask const* get_table()
    {
      typedef ctype<char> cctype;
      static const cctype::mask *const_rc = cctype::classic_table();

      static cctype::mask rc[cctype::table_size];
      memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

      for (char del : _delims)
      {
        rc[del] = ctype_base::space;
      }

      return &rc[0];
    }
  };

  stringstream ss(input);
  ss.imbue(locale(locale(), new tokens()));
  return vector<T>(istream_iterator<T>{ ss }, istream_iterator<T>());
}

void parse_seq(const string& s, list<vector<int>>& seq)
{
  size_t pos = s.find('(', 1);
  size_t prev_pos = 0;

  if (pos == string::npos)
  {
    auto seq_part = split_string<int, ')', '('>(s);
    seq.emplace_back(seq_part);
    return;
  }

  while (true)
  {
    auto seq_part = split_string<int, ')', '('>(s.substr(prev_pos, pos));
    seq.push_back(seq_part);

    prev_pos = pos;
    pos = s.find('(', pos + 1);

    if (pos == string::npos)
    {
      auto seq_part = split_string<int, ')', '('>(s.substr(prev_pos));
      seq.emplace_back(seq_part);
      return;
    }
  }
}

size_t breakpoint_distance(const list<vector<int>>& Q, const list<vector<int>>& P);

int main()
{
  list<vector<int>> Q, P;

  string input;
  getline(cin, input);
  parse_seq(input, Q);
  
  getline(cin, input);
  parse_seq(input, P);

  cout << breakpoint_distance(Q, P);

  return 0;
}