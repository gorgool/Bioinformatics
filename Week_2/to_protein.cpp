#include <string>
#include <cassert>
#include <map>

using namespace std;

map<string, char> protein_tbl
{
  { "UUU", 'F' }, { "UUC", 'F' },
  { "UUA", 'L' }, { "UUG", 'L' },
  { "UCU", 'S' }, { "UCC", 'S' }, { "UCG", 'S' }, { "UCA", 'S' }, { "AGU", 'S' }, { "AGC", 'S' },
  { "UGG", 'W' },
  { "UAU", 'Y' }, { "UAC", 'Y' },
  { "UGU", 'C' }, { "UGC", 'C' },
  { "CUU", 'L' }, { "CUC", 'L' }, { "CUA", 'L' }, { "CUG", 'L' },
  { "CCU", 'P' }, { "CCC", 'P' }, { "CCA", 'P' }, { "CCG", 'P' },
  { "CAU", 'H' }, { "CAC", 'H' },
  { "CAA", 'Q' }, { "CAG", 'Q' },
  { "CGU", 'R' }, { "CGC", 'R' }, { "CGA", 'R' }, { "CGG", 'R' }, { "AGA", 'R' }, { "AGG", 'R' },
  { "AUU", 'I' }, { "AUC", 'I' }, { "AUA", 'I' },
  { "AUG", 'M' },
  { "ACU", 'T' }, { "ACC", 'T' }, { "ACA", 'T' }, { "ACG", 'T' },
  { "AAU", 'N' }, { "AAC", 'N' },
  { "AAA", 'K' }, { "AAG", 'K' },
  { "GUU", 'V' }, { "GUC", 'V' }, { "GUA", 'V' }, { "GUG", 'V' },
  { "GCU", 'A' }, { "GCC", 'A' }, { "GCA", 'A' }, { "GCG", 'A' },
  { "GAU", 'D' }, { "GAC", 'D' },
  { "GAA", 'E' }, { "GAG", 'E' },
  { "GGU", 'G' }, { "GGC", 'G' }, { "GGA", 'G' }, { "GGG", 'G' },
  { "UAA", 'X' }, { "UAG", 'X' }, { "UGA", 'X' }  // Stop codone
};

string to_protein(const string& input_str)
{
  string ret;
  size_t pos = 0;
  string codone(input_str.substr(pos, 3));

  while (pos + 3 < input_str.length() && protein_tbl[codone] != 'X')
  {
    ret += protein_tbl[codone];

    pos += 3;
    codone = input_str.substr(pos, 3);
  }

  return ret;
}