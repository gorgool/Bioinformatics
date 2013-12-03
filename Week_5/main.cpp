#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "graph.h"
#include <string>

using namespace std;

string get_universal_string(const size_t k);

int main()
{
  size_t k;
  cin >> k;
 
  cout << get_universal_string(k);

  return 0;
}