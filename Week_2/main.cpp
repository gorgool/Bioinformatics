#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

string to_protein(const string& input_str);
vector<string> find_protein_encode(const string& genome, const string& protein);

int main()
{
  string input_str;
  cin >> input_str;
  //input_str = "TTCGAATCCGTATTGCTTTGGTTGGTTGAGAGAGCGACCTACATTGCTAGTCAGAATAGGTGATTCACACAAAGCTTAGCACCTGGGCAGCACCCCGTGATGTAAACCTATGGGAACTAAGGGAGTCCTGCGGTTTTAGCCAGCAAGCGAGCCGGCAGGAACACTCATACCATCGGACGCGTTTGACGCCTCCCCGGAAAGGAAGTATTTGAGCCTCATTATTACGTATTGCCCGTTAGTCGACAAATCAAGCCCTCGTACGCAGCTTATTCGTACGACGTGGAGGCGTTCCCACGGGCCTAACACGATTGGAACACCACCATAGTAGTGTGGTTCAAATACCTCCTTTGGAGATCTAGAGCTTCACTCTGATTCTAGAGGCAACTTTACAATCGCTCTACGAAATTGTATGGACATCATCAACCGGATATTCTGGGGCGGTAGAATTTCTTTTGTTCGAATCGCTCTAGGCCAGGATCAAATTAATTGAATTGCGGACTCAAGGATCGCGATAGCCGACACATCGGACGCTGTAGAAAGCCAGTCTCTGGATTTAATCCACCCTCTATGTTTGACAAAGCACTAAAACGGGATAGTTTCGGGTGGTATAAGTTTCCCAAGACGATTGCATCGCAATTCATCAACAACCATGAACTTACTGTTTTAGTACTTCCACACACCTTGTTAAATTACGCCTTTACTTCATGTTGCGGTGTGTGTTAGATAGTGTGCAGCTACAAGTCTACCGCCATCGCAGCTCGGGATACCGGCAGATGAGATGGTCCTGAGCTCGTACCGGACTCAAACTTTTTCCTTTACTACCTAGGAATCGCCCATGCGAATTTGTCGGACACACACCATTACATTAACGTCACAACAGCTACTGTTAGAATTTTGCTCTTGCAAATCCTGGAAAGAGTTAAAAAAACTCTTCCGCGCGCCAATAGGGTAAATAATAGATAGCCAGACGGCTGTAAGAGGTGATGACATTTGCAACAATCATGCTGTCGCATCTTCCGCAAGTTCATGTCGCGCCTAGGCAATGGATCTGCGAATGGGGGCCACGGGGTATGAACTACGGAATTCTAAGAAAGTTGCCATCCAGAGTTAAGGGTTTGAGGCTAGTTGCATCGCTGGTAACGAACTACCTCATTACTTGGACGCGCAGTGTGACTTCACTCCTGTATAGCGATGATGCCAAGCAGGAATTAGCAAATCTGAAGAGCGTTTCCAAACTGGCCACTTGGACTGACACCTATCGCGGGGGATTTCAGCGCGTGTCGCTCTCACATGAGAGCTGCCGTCAGGAGCGGTAGAGTTTAGAGAGGAATGCGACAAACTCCCTATTCACCTCTCTGGTGATGTAAGGATATTTACGCTTAGTTCTATGCCAGGCTTAGGGCCTCTCGGAACTTTGGTGAGTCCTTATTAATTGATGCTACCTCTCCCTTACCTTCGCCCCAAGTCACGTAGAAGTACTCAATCCTGCTACATGATAATCAAATATTTCCAACGTTGGGAAATCGGTGACATCACATACTAGTTAAGAAACCACTGTCAGTGAACTTATATCCGGGGGAGAAAATCTACTAACTTACATACGCTGTGCGAGCAGTTTTCATTATAAGAAAATATACTCCCGAGGTACCGCATCAAGCACGACATTCCCGGAGAGCATAACATTTCGGTGCACCTGCTTTTGTGCGCTTGCTTGCGGTTATTTATAAACTACGCACAAGGCGCAAACCGCAGTGCGCATGTTTTCTCCGCCTGGCTAGAACTCGACATTCTCGTCAACGCCAATCTATGTGAGAGGATTTAGACCTCTGTGAAAACGAGTCCCTCTATAGAATAAATACCCAGATGCCAATGGGGGTTCTATCCGATGGCAGTGCATGGAGTGGTGGCTCCAGATTAAGGATGAGGAGAGGTAAAGATAACAGTTCGGTCGCCACGACGCGTTGCCAATCGAAATATCAGTACTAAAAGGCCCACCGCTCCGCTTTAGTCCGACTTTACATCCTGTGGAAATTGTCGAACGGAGGCTACATCGGGCTATATGAGTGTGAAAACCTATACTTCTCGCGTCGTTACTCAGTGCCGGTCTCCTGTTTCCCCCAGTCTTACGTACCCTTATTGATATTTGCTTCACGTTGAAACGTCCTAACGCAGCGTAAAGAGGTGTTTGAACCTCATTACTATAAAATCGCGATCGAAGGTAGACTACATACGCAAACGCCGAAACCCTCAGTTGGCCTTGTTGCAAGTATGGAACGTTGTAAAATTTTTCCTAGACGTTGAGCTATCGGTACAAGGTCGTTAGCGTCCTTACCCTTCACTTATATGCCCGACAAAACGCGGGTCCTAGTGCAGTGGTGGGAGCTTGGAATCCCGCAATACAAGGACAACCTGTATCTCGTTCGGCGTTCCGCGATCACTCGATCCCGAACCACTCCAAGCCTGGTTGATCAGCAAAAGCGGAAGGATGGATAAAGGGCTACTGGTTAATGGATGTAAACTTCCAATGATGAAATCCTGGAAACGAGGGATCGGGTTACGGTGGCGAACGGGGTACGGCAACGTGGCTATCTAGAGCCCGACGTTACGACTCATGTACATGCTGCTACGTGGTTGAAGCTGACGTTCAGATGAAGCAGTACTGAGTCCTAGGGCTTACTACTACTCCAATAGGTCTGGCCGGCCAGATACAAAAGTTCGTGGCGGCTCACCCCCTTTCTGGCGGGTGTAGCTTGCTGACCGGTTTGCTCGATAACACAGGCTAGCGAATAGTAATGAGGTTCGAAAACCTCTTTCCAACGACTGAAAGGGTCTACACGAACTATCTACATTTCCCCGCCCATGTCCTTCCGTCTGGTTGCTTCTGGAGATCCTTTCGCATTATACCGCAGCGTAGTGGCTCTGGCATATATGAAAAAATCCTTCTGTGGGTATTTGTGCCATCACTTATTGTTCGTACCGATATGGGATTACAAGTGCGATGTGATAATAAGCGAAGAAGCCAACATGTTACACTGTTCATGCGCTCCGGGTAATGTGCGGGCACCATGCTCAGTTCCCGCTCGCAGTTGTCACTGTCCCTGTTTCGGCACCATAATCAACATTTCCACGGCCACGCTGGTGAATAACCGAGGATACCGAAGTACAGCAAGAATGAGAGCGGGACTCCTCCATCTGCTTGTAATACGCCTTCAAGATAGTCCATAAAACGGTCGGGGTCTTGTTTCGGACTAGCCGCTTTGAAACGGTGCATAGTTGTGTCAAGTGTGGACATTGGCTTTCTATCCTCGTCAGCGATCCTCGGAAAGACTCGGGCAGTCGCCCCGAATCGTAATTAGGTAGTAGTGCGGCTCAAAAACTTCCTTCGACCTAACCGCTATAATGTTCGTAGATATAAATTTCGTTTCAGTATTAACAGGCGCACCGTATATATACGGAATGGTGTCGCCCCATTAGCTGCTCGCCAATATTTATCTAAGACCGCGCGCGTCTAGCGCCTTTAGTAGTTGCACCCGAGTATAGTAATGGGGTTCGAAGACTTCCTTCGCAAGGCTGCCATACTGTATCACAAGTACTGACGGAGCCCCGGAGGAGTGCAGGATACGGCAAAGGAGACCATTACCGGGGCATGAGTCCAAGTTAGCCCGTTAGGTGAAGGACGCTGATACAATAGTGAATCCGTTACTGAAAGGTTTAGAAGACCGGGGGGCTCGCACTAGGTCCAAATATTATGAACCCTACTCCTGCAACTGAATTGGCCGTCCAGGCGATATTTAAAAGGGGTTACTAGCAGGTTCATCGGAGCCCGTACTCCTTCCGGGCATAGTCGTTCGACGGGTAGAAATTCATCCAGTCGTGCCGGATACCCCGAGAATACCCCTATTTTTTGATCCTTCACCATCATCGTCCGCGGACTCATCTAAGTACCTCAGACCGAAACTGTTATCGTAGCGAAGAGCGAACTCGAATGACATCGCTTGTCCAACAGGGAAAATATGTAAAGTATATGCAGATTATTATAGGAAGATCACAAACTCCATCGCGCCTAGGCCAAAGACTTGCCAGAACAACATCTCTTCCAGAGCAAGGAAGTGTTTGAACCTCACTATTATCGAGAGAAGTCCCATGAATTTATAATAGTGAGGCTCAAAAACTTCCTTCATCGTCGGGCGCTGGGGCGAGCTAGGCTTCCCTAGCCGTCATTACTGTACCCACGCCAAATATTATCGAGTATGACTACGAAGCCTTCACAAGGGGCTGCAGTAACAGACTAACTCACGCACACCGCAACTACTATCCTAAATTGAGTAAGAAAACCCTCGACTATAGCCCTTGAATTAAATTCGCTATTTAAGGAAGACCGCGCTTCCGCTCGCCCGCGTATAGTTTACCAGTCCCAAGAAACATGTGTGGCCAGCCTACCTGAAGAGCTGTGTAAAGATAGATTCTGACATCCTCAAAAAGAAGTTTTCGAGCCGCACTACTACGCACGGAGCTCCGTTATTCAAGGCATGTCGAAGTACAGCGTGGTGGCCTCTCCTGTTCTCCACCCCAGCTAAACCCACCTTGTTCGAATTCGCGCAACTGTATATGACATGAACACTTACAGGGGTTAGAAGTTACTGCAACCAAGATAGAGCTCGTCGAAGTAATAGTGCGGTTCAAAAACTTCCTTCAATTGGTCTCATCACTTAAATTTAAGAGCTATTGTGAGTACAGGTACGGATGCGGCTTCAGTGGATCTTCAGCATTCATTCCTTGTAGTAATGGGGTTCGAAGACTTCCTTGCCAGGGTACCAAACAAGTCTTGCGCATCCTCCTCCCTAAGGAGGTATTTGAACCCCACTATTACCCACGATAGAACATGCAGGGTTTGATAGTGGAACACCTTTTAGAATCTGGGGATAAATTCCCAGGACTAATGTATGGCTGTAGTAATGAGGTTCAAAAACCTCCTTTTCAGGTGGATCGCAGGCCGTGCTGCCTCACAAGCTGGGACGCCGTCCACGGTATAGCCGGCGTCGGCAGTTACTGTGAAATAGCGGAAACTCGATCCCAATATATCATCTTACGTTTGGCGCCCAATAGTCGCCCAGTACCCGTTGACAGTTCTTTAACTCGGCTTAGAACTACTAGACAGGTTCAACCGAACCTTGCCCTAGTTCCCACTCCCGTAATTCATTTGGGTTTGCATCTAGAATACTGGAGGGTGCATAGACGAAACGTGTACGTCGGAGAAAACGTAAGAAAATGACCTAGACTCATAGTAGTGAGGTTCGAAGACTTCCTTTCAGTGAAATCGATCCACCACTCGCCGCGAAGAGATAATAGCATAGAGCACAAGTGCGCGAGTAGAGAAAAAGGCTATCCCAACCGGGCACGTCCTTCGTGTTTGGCGTTTACATACGGCACCCCGTTTCTGCACGTTAACCGTCTAGTATCCAACGGTGGATGGCGGACGCTAGACTATAGATATGAGATATCGAGACCTGGAGCTGGGTGTGGCTGCAGCCCGGGTCATTGCGGGCTGTGAATTCAAGGGCATGTAAACAAGCGTATATCGAACAGTGGATGGGCACCTGCAATACTCACGGTAGAGTTAGCTCACAGGATTCACGTTGAGGACTATGAGTCCCTCTTCGCTAGCAGTCTGGGGGGATATGGAGTTTAATAGCTTGACGTAGTAATGGGGCTCAAACACCTCTTTGTGTGAGCACAGCTACTTGCATTAAGAGATTCTAAACAGCGATCATCTCGGCTATTTCGGGCCAGCCTTTTCGGCAGGATGTTATGTAGCATTTCTGGAAGCTTCCCCCTCGAATCTACTAGTGGTGAGAAGATGCCCCACCGATATTACTCTTTAATCTTGAGAAACCTAAAACCGATCTGACCTCAGACGGGCGGCTCCCACCCGAGGATAAACTCGTCAATAATAATGTGGCTCGAACACTTCTTTTCTCACTAGGCTTTTACGACACGCCACATGTATTTAAGCATCTACCTAACTTGTGTCTGCTGATATACAGCGCATTCTACGCCCAACCTACCAATTACTTCAACGTAGTGCGTGGCTAAAATTCAGGGGAGCTTCATCTCTGTCTTAATTTGAAGGTTCTTCCGGGGCGTTTGGGAATCTTCGTGCCTTTTGCGAGGTTAAGGTATCAAAGAAGTTTTCGAACCACATTATTACCGCCTTAAGCACCGGCGCATCCTGCTCGTGACAACTCTACCCTGCCCTGATAAAGGCACTGAACGTTCCAGAGAGTGCATCATTGACACGCGAGCAGGCCACAGTAGCCACAAGACGTATGGGTGATTATAGAATTGGTGGAGGTGTTGTTAACGATCAGGAGGACATTAGTGGGAGTTAGGAAAGACCCTATGTTCTCTCTATCGCGGACTTGTAACTTGACAAGCAAAAGGGTAAGAGAGCTGCACACCGAAGCAGGCCCTTCCTATACCTGTTTTTCCTACGCGTAGAGAGGAATCCAGAAAGGTGATAATTGGCATTCGATGAAAAAACAGTGTGCCACTGACTTAGTTCTATATGTGAAGAGCCTGTTAGCACGTGACGGCGGCCTTGGTATAGAGCCCTTAATGGTCTCCATCGCGTAGTAATGGGGCTCGAAAACCTCCTTACTTGGGATTGCGTGGCCTCCTTGTGAGTCATACACAAGGCTTAGGGCTATGGGGCGATACACTCCTTTTCGCGGCGCATGGGGCGGTGATGCCTACATAGTAGTAGTGACTGCCTTTCTGGGGGGCTATTTGTGGATGACCAACACCTGACCAGCGATGCAATCGCTAGGGGAGGTACACCTCTCATATGTTACAACAATCACCGAATTGTGTTTCGAATTCGAATCAAGTTTGCGGTGTCGACCAGATCTGGTCTTGCTGCCATACCGGGTTCGCCGCCTCCGGTGGATAGAACTGCATCTTAAGACATCTGGACCCAGCGGTAAGTAGCGGGAAGAGTTTAGAGTCATTCGTACAACTACAGGCTAAGGGCTTACTGGGGAGTTGTTGTAGGGCATAAAGATCGCCCCATGACTTTTCGTACTTTCCCCGATAGTTCACTCGCAGCGAGCTGCGGCTGGGCTTCGCCACACGAGTACGGGCAACATTTATCTCCTCTAATCACTGGGCACCGCGCGAGGAAATAGAAAACCCTAATCAGTGCTCATGGGCGCATCTATTGGTCTCCGCATGCACGATGCCGCGGAGTGCTTAGTTGTCCCTGCATAATCTTCGTAGATGTATAAGAGATTACCTATTTATTCGGTTTCGGTTCTAGACGTACCTTGCCGCATGAGTATAGGCTAATGAACTGAGTTGGCGCCAGAGGGAAAGGCATAATAATGCGGCTCGAATACTTCCTTAAGGAAGTATTCGAACCACATTACTAT";
  
  string proteins;
  cin >> proteins;
  //proteins = "KEVFEPHYY";

  auto ret = find_protein_encode(input_str, proteins);
    
  for (const auto& item : ret)
    cout << item << endl;

  return 0;
}
