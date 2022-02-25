#include <bits/stdc++.h>

template <int word_len, typename T> void iter_accuracy_possibilities(T fn, std::array<int, word_len> combonation, int iter_number = word_len) {
  if (iter_number == 0) {
    fn(combonation);
  } else {
    for (int i{}; i < 3; i++) {
      combonation[iter_number - 1] = i;
      iter_accuracy_possibilities<word_len>(fn, combonation, iter_number - 1);
    }
  }
}

/*
  fn should be formatted as fn(combonation, *args)
*/

template <int w_len> float information_eval(std::ifstream& inp_file, std::string guess, Wordle_game<w_len>& wr_game) {
  float information{0};
  int total_possibilities{0};
  std::array<int, w_len> cmb;
  typedef std::pair<std::array<int, w_len>, unsigned long> acc_pair_type;
  std::unordered_set<int> total_combonations;
  
  map_set<acc_pair_type> acc_arr_to_combonations;

  int tw{filter_f(inp_file, [](std::string _){ return true; }).size()};
  std::cout << '\n' << tw << '\n';
 
  iter_accuracy_possibilities<w_len>([
  &wr_game, 
  &guess, 
  &total_possibilities, 
  &total_combonations, 
  &inp_file
  ](std::array<int, w_len>& word_acc_arr){
    Wordle_game<w_len> w_game_cpy{wr_game};
    W_word<w_len> v(guess, word_acc_arr);
    
    //std::cout << v << '\n';
    w_game_cpy.append_w_word(v);

    //std::cout << w_game_cpy;

    unsigned long tw{filter_f(inp_file, [](std::string _){ return true; }).size()};
    std::cout << '\n' << tw << '\n';
    
    std::unordered_set<std::string> p{w_game_cpy.possibilities(inp_file)};
    
    for (const auto& wrd: p) {
      std::cout << wrd << ' ';
    }
    
    unsigned long possibilities{static_cast<int>(p.size())};
    total_possibilities += possibilities;
    //std::cout << possibilities << '\n';
    total_combonations.insert(possibilities);
  }, cmb);
  
  for (const int& combonations: total_combonations) {
    float prob{static_cast<float>(combonations) / static_cast<float>(total_possibilities)};
    information -= prob * std::log2(prob);
  }
  
  return information;
}

template <int w_len, int guesses = 6> std::map<float, std::string> w_game_eval(Wordle_game<w_len, guesses> w_game, std::ifstream& word_file) {
  std::map<float, std::string> eval_to_word;
  iter_file_by_line(word_file, [&eval_to_word, &w_game, &word_file](std::string word){
    word = word.substr(0, w_len);
    eval_to_word.emplace(information_eval(word, w_game, word_file), word);
  });
  
  return eval_to_word;
}




template <int len, int guesses = 6> void solve_game_io(std::ifstream& word_file) {
  Wordle_game<len, guesses> evaluated_game("");
  while (1) {
    std::map<float, std::string> word_eval{w_game_eval<len, guesses>(evaluated_game, word_file)};
    int iter_times{std::min(5, static_cast<int>(word_eval.size()))};
    auto itr{word_eval.end()};
    std::cout << iter_times << '\n';
    
    for (int i{}; i < iter_times; i++) {
      auto&[ev, word]{*(--itr)};
      std::cout << word << ' ' << ev << '\n';
    }
    
    W_word<len> acc_guess{w_word_io<len>()};
    evaluated_game.append_w_word(acc_guess);
  }
}
