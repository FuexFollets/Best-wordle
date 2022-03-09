#include "evaluation.hpp"

int main() {
  std::ifstream word_file{"5lw.txt"};
  const word_set ws{words_from_file<5>(word_file)};
  std::cout << "Works maybe?\n";

  
  float ev{information_eval<5>("hello", ws)};
  std::cout << ev;////

  
  std::cout << '\n' << "Code compilied" << '\n';  
  return 0;//
}
