#include <string>
#include <vector>

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

string to_protein(const string& rna);

vector<string> find_protein_encode(const string& genome, const string& proteins)
{
  string rna(dna_to_rna(genome));

  string protein_str = to_protein(rna);

  vector<string> ret;

  int pos = -1;

  while (true)
  {
    pos = protein_str.find(proteins, pos + 1);

    if (pos == string::npos)
      break;

    ret.push_back(string(genome.begin() + pos, genome.begin() + pos + proteins.length() * 3));\
  }

  return ret;
}