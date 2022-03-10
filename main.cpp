#include "evaluation.hpp"

int main() {
  std::ifstream word_file{"5lw.txt"};
  const word_set ws{words_from_file<5>(word_file)};
  std::cout << "Works maybe?\n";

  std::string temp;
  std::cout << "tell me a word: ";
  std::getline(std::cin, temp);
  
  float ev{information_eval<5>(temp, ws)};
  std::cout << ev;//

  
  /*
  auto evws{ww_all_eval<5>(ws)};

  auto itr{evws.end()};

  for (int i; i < 10; i++) {
    auto&[ev, word]{*(--itr)};
    std::cout << word << " : " << ev << '\n';
  }
*/
  //solve_game_io<5>(ws);
  
  
  std::cout << '\n' << "Code compilied" << '\n';  
  return 0;
}
