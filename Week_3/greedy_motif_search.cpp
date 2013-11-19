#include <string>
#include <vector>
#include <random>
#include <map>
#include <numeric>
#include <cassert>

using namespace std;

string most_prob_kmer(const string& dna, const size_t k, const vector<map<char, double>>& profile);

static vector<map<char, double>> eval_profile(const vector<string>& motifs)
{
  vector<map<char, double>> ret;
  for (size_t idx = 0; idx < motifs[0].length(); ++idx)
  {
    map<char, double> entry{ { 'A', 1.0 }, { 'T', 1.0 }, { 'C', 1.0 }, { 'G', 1.0 } };
    for (const auto& dna_string : motifs)
    {
      entry[dna_string[idx]] += 1.0;
    }

    size_t sum = accumulate(entry.begin(), entry.end(), 0.0, [](double total, const pair<char, double>& item){ return total + item.second;});
    
    for (auto& item : entry)
    {
      item.second = item.second / sum;
    }

    ret.push_back(move(entry));
  }

  return ret;
}

static size_t max_num_misses(const string& input, const string& original)
{
  assert(input.length() == original.length());

  size_t num = 0;
  for (size_t i = 0; i < input.length(); i++)
  {
    if (input[i] != original[i])
      num++;
  }

  return num;
}

// Hamming distance
static size_t score(const vector<string>& motifs)
{
  string consensus;
  for (size_t idx = 0; idx < motifs[0].length(); ++idx)
  {
    map<char, size_t> tbl{ { 'A', 0 }, { 'T', 0 }, { 'C', 0 }, { 'G', 0 } };
    for (const auto& motif : motifs)
    {
      tbl[motif[idx]]++;
    }

    pair<char, size_t> most_freq = *tbl.begin();
    for (const auto& item : tbl)
    {
      if (item.second > most_freq.second)
        most_freq = item;
    }

    consensus += most_freq.first;
  }

  size_t ret = 0;
  for (const auto& motif : motifs)
  {
    ret += max_num_misses(motif, consensus);
  }

  return ret;
}

vector<string> greedy_motif_search(const vector<string>& dna, const size_t k, const size_t t)
{
  vector<string> best_motifs;

  {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> range(0, dna[0].length() - k + 1);
    for (const auto& dna_string : dna)
    {
      best_motifs.push_back(dna_string.substr(range(gen), k));
    }
  }

  for (size_t pos = 0; pos < dna[0].length() - k + 1; pos++)
  {
    vector<string> motifs;

    string motif = dna[0].substr(pos, k);
    motifs.push_back(motif);

    for (size_t i = 1; i < t; i++)
    {
      auto profile = eval_profile(motifs);
      motifs.push_back(most_prob_kmer(dna[i], k, profile));
    }

    if (score(motifs) < score(best_motifs))
    {
      best_motifs = move(motifs);
    }
  }

  return best_motifs;
}