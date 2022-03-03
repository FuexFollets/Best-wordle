#include "evaluation.hpp"



int main() {
  std::ifstream word_file{"5lw.txt"};
  const std::unordered_set<std::string> word_set{words_from_file<5>(word_file)};

  std::cout << '\n' << "Code compilied" << '\n';  
  return 0;
}
