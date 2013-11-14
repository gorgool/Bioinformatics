#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <chrono>
#include <utility>


using namespace std;


template <
  typename RetT,
  typename FuncT,
  typename ... argsT >
  double exec_time(RetT& ret, FuncT && func, argsT& ... params)
{
  using namespace std::chrono;
  high_resolution_clock::time_point start_time = high_resolution_clock::now();
  ret = func(forward<argsT>(params) ...);
  return duration_cast<duration< double >>(high_resolution_clock::now() - start_time).count();
}

vector<size_t> convolution_leaderboard_sequencing(vector<size_t>& mass_spectrum, const size_t M, const size_t N);

int main()
{
  size_t M, N;
  cin >> M;
  cin.ignore();
  cin >> N;
  cin.ignore();
  //M = 20;
  //N = 60;

  string input_str;
  getline(cin, input_str);
  //input_str = "57 57 71 99 129 137 170 186 194 208 228 265 285 299 307 323 356 364 394 422 493";

  istringstream iss(input_str);
  vector<size_t> mass_spectrum{ istream_iterator<size_t>{iss},
    istream_iterator<size_t>{} };

  vector<size_t> ret;
  auto time_to_run = exec_time(ret, convolution_leaderboard_sequencing, mass_spectrum, M, N);

  for (size_t i = 0; i < ret.size(); i++)
  {
    cout << ret[i];
    if (i < ret.size() - 1)
      cout << "-";
  }
  cout << endl;
  cin.ignore();
  return 0;
}
