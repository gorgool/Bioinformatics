#include <string>
#include <vector>
#include <map>
#include <random>
#include <thread>
#include <iostream>

using namespace std;

string most_prob_kmer(const string& dna, const size_t k, const vector<map<char, double>>& profile);
vector<map<char, double>> eval_profile(const vector<string>& motifs);
size_t score(const vector<string>& motifs);

static vector<string> gen_motifs(const vector<string>& dna, const vector<map<char, double>>& profile, const size_t k)
{
  vector<string> ret;
  for (const auto& dna_string : dna)
  {
    ret.push_back(most_prob_kmer(dna_string, k, profile));
  }
  return ret;
}

vector<string> randomized_motif_search_run(const vector<string>& dna, const size_t k, const size_t t)
{
  vector<string> best_motifs, motifs;

  {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> range(0, dna[0].length() - k);
    for (const auto& dna_string : dna)
    {
      best_motifs.push_back(dna_string.substr(range(gen), k));
    }
  }

  motifs = best_motifs;

  while (true)
  {
    auto profile = eval_profile(motifs);
    motifs = gen_motifs(dna, profile, k);

    if (score(motifs) < score(best_motifs))
      best_motifs = motifs;
    else
      break;
  }

  return best_motifs;
}

vector<string> randomized_motif_search(const vector<string>& dna, const size_t k, const size_t t)
{
  vector<string> ret = randomized_motif_search_run(dna, k, t);

  for (size_t i = 0; i < 1000; i++)
  {
    auto iter = randomized_motif_search_run(dna, k, t);
    if (score(iter) < score(ret))
      ret = iter;
  }

  return ret;
}