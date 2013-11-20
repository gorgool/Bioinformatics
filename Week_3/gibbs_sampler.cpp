#include <string>
#include <vector>
#include <random>
#include <map>
#include <unordered_map>
#include <numeric>
#include <iostream>

using namespace std;

size_t score(const vector<string>& motifs);

static vector<map<char, double>> eval_profile(const vector<string>& motifs, const size_t ignore_idx)
{
  vector<map<char, double>> ret;

  for (size_t idx = 0; idx < motifs[0].length(); ++idx)
  {
    // With Laplace smoothing
    map<char, double> entry{ { 'A', 1.0 }, { 'T', 1.0 }, { 'C', 1.0 }, { 'G', 1.0 } };

    for (size_t i = 0; i < motifs.size(); ++i)
    {
      if (i == ignore_idx)
        continue;
      entry[motifs[i][idx]] += 1.0;
    }

    size_t sum = accumulate(entry.begin(), entry.end(), 0.0, [](double total, const pair<char, double>& item){ return total + item.second; });

    for (auto& item : entry)
    {
      item.second = item.second / sum;
    }

    ret.push_back(move(entry));
  }

  return ret;
}

static string random_motif(vector<map<char, double>>& profile, const string& dna)
{
  const size_t k = profile.size();

  // Table {"ATTGCCAT", 0.000056}, {"TGCAACAT", 0.00065898}, ...
  unordered_map<string, double> prob_tbl;
  for (size_t pos = 0; pos < dna.length() - k + 1; pos++)
  {
    // K-mer
    string substr = dna.substr(pos, k);

    // Probability = P(1st nucleotide) * P(2nd nucleotide) * ...
    double prob = 1.0;
    for (size_t i = 0; i < k; ++i)
      prob = prob * profile[i][substr[i]];

    // Add to table
    if (prob_tbl.find(substr) == prob_tbl.end())
      prob_tbl.insert({ substr, prob });
    else
      prob_tbl[substr] += prob;
  }

  // Normalize
  double sum = accumulate(prob_tbl.begin(), prob_tbl.end(), 0.0, [](double total, const pair<string, double>& item){ return total + item.second; });
  for (auto item : prob_tbl)
    item.second = item.second / sum;

  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> roll(0, 1.0);
  // Roll dice
  double point = roll(gen);

  double head = 0.0;
  for (auto it = prob_tbl.begin(); it != prob_tbl.end(); ++it)
  {
    if (point < head)
      return (--it)->first;
    head += it->second;
  }

  return prob_tbl.rbegin()->first;
}

vector<string> gibbs_sampler_run(const vector<string>& dna, const size_t k, const size_t t, const size_t N)
{
  vector<string> best_motifs, motifs;

  random_device rd;
  mt19937 gen(rd());

  { 
    uniform_int_distribution<size_t> range(0, dna[0].length() - k);
    for (const auto& dna_string : dna)
    {
      best_motifs.push_back(dna_string.substr(range(gen), k));
    }
  }

  motifs = best_motifs;

  uniform_int_distribution<size_t> range(0, t - 1);

  for (size_t i = 0; i < N; i++)
  {
    size_t idx = range(gen);
    auto profile = eval_profile(motifs, idx);
    motifs[idx] = random_motif(profile, dna[idx]);
    if (score(motifs) < score(best_motifs))
      best_motifs = motifs;
  }
  return best_motifs;
}

vector<string> gibbs_sampler(const vector<string>& dna, const size_t k, const size_t t, const size_t N)
{
  vector<string> ret = gibbs_sampler_run(dna, k, t, N);

  for (size_t i = 0; i < 20; i++)
  {
    auto iter = gibbs_sampler_run(dna, k, t, N);
    if (score(iter) < score(ret))
      ret = iter;
  }
  cout << score(ret) << endl;

  return ret;
}