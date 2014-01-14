#include <iostream>
#include <string>
#include <vector>
#include "trie.h"

using namespace std;

string longest_repeat(const string& text);

int main()
{
  string text;
  cin >> text;

  cout << longest_repeat(text);
  
  return 0;
}