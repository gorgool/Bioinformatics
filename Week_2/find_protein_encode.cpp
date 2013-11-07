#include <string>
#include <vector>
#include <map>

using namespace std;

static string dna_to_rna(const string& dna)
{
  string ret(dna);
  for (auto& nucleotide : ret)
  {
    if (nucleotide == 'T')
      nucleotide = 'U';
  }
  return ret;
}

map<char, char> pairs =
{
  make_pair('A', 'T'),
  make_pair('T', 'A'),
  make_pair('C', 'G'),
  make_pair('G', 'C')
};

static string reverse_complement(const string& input_str)
{
  string ret(input_str);
  for (size_t i = 0; i < ret.length(); i++)
  {
    ret[i] = pairs[ret[i]];
  }
  return string(ret.rbegin(), ret.rend());
}

extern map<string, char> protein_tbl;
static string to_protein(const string& input_str)
{
  string ret;
  size_t pos = 0;
  string codone(input_str.substr(pos, 3));

  while (pos + 3 <= input_str.length())
  {
    ret += protein_tbl[codone];

    pos += 3;
    codone = input_str.substr(pos, 3);
  }

  return ret;
}

vector<string> find_protein_encode(const string& genome, const string& proteins)
{
  vector<string> ret;

  string substr, peptide, reverse_peptide;
  for (size_t i = 0; i < genome.length(); i++)
  {
    substr = genome.substr(i, proteins.length() * 3);

    peptide = to_protein(dna_to_rna(substr));
    reverse_peptide = to_protein(dna_to_rna(reverse_complement(substr)));

    if (
      strcmp(peptide.c_str(), proteins.c_str()) == 0 ||
      strcmp(reverse_peptide.c_str(), proteins.c_str()) == 0)
    {
      ret.push_back(substr);
    }
  }

  return ret;
}