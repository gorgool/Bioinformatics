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

vector<size_t> spectrum_convolution(vector<size_t>& mass_spectrum);

int main()
{

  string input_str;
  getline(cin, input_str);
  //input_str = "465 473 998 257 0 385 664 707 147 929 87 450 748 938 998 768 234 722 851 113 700 957 265 284 250 137 317 801 128 820 321 612 956 434 534 621 651 129 421 337 216 699 347 101 464 601 87 563 738 635 386 972 620 851 948 200 156 571 551 522 828 984 514 378 363 484 855 869 835 234 1085 764 230 885";

  istringstream iss(input_str);
  vector<size_t> mass_spectrum{ istream_iterator<size_t>{iss},
    istream_iterator<size_t>{} };

  vector<size_t> ret;
  auto time_to_run = exec_time(ret, spectrum_convolution, mass_spectrum);

  for (size_t i = 0; i < ret.size(); i++)
  {
    cout << ret[i];
    if (i < ret.size() - 1)
      cout << " ";
  }
  cout << endl;
  return 0;
}
