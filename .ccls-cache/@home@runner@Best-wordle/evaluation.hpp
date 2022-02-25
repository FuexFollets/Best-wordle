#include <bits/stdc++.h>

template <int word_len, typename T>
void iter_accuracy_possibilities(T& fn, std::array<int, word_len> combonation, int iter_number = word_len) {
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

template <int w_len> float information_eval(std::string guess, Wordle_game<w_len>& W_game, std::ifstream& word_file) {
  float information{};
  int total_possibilities{};
  std::array<int, w_len> cmb;
  map_set<std::pair<std::array<int, w_len>, int>> acc_arr_to_combonations;
  
  auto set_inf{[
  &W_game, 
  &guess, 
  &total_possibilities, 
  &acc_arr_to_combonations, 
  &word_file
  ](std::array<int, w_len> word_acc_arr){
    auto w_game_cpy{W_game};
    auto v{W_word<w_len>(guess, word_acc_arr)};
    //std::cout << v << '\n';
    w_game_cpy.append_w_word(v);
    std::unordered_set<std::string> p{w_game_cpy.possibilities(word_file)};
    for (const std::string& wrd: p) {
      std::cout << wrd << ' ';
    }
    int possibilities{static_cast<int>(p.size())};
    //std::cout << w_game_cpy << '\n';
    total_possibilities += possibilities;
    std::cout << possibilities << '\n';
    acc_arr_to_combonations.append(std::pair<std::array<int, w_len>, int>(word_acc_arr, possibilities));
  }};

  iter_accuracy_possibilities<w_len>(set_inf, cmb);
  
  for (const auto&[_, pair_obj]: acc_arr_to_combonations) {
    auto&[acc_arr, combonations] = pair_obj;
    float prob{static_cast<float>(combonations) / static_cast<float>(total_possibilities)};
    information += prob + std::log2(prob);
  }
  
  return -information;
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
