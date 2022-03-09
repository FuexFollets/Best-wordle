#include "wordle_objects.hpp"

template <int word_len, typename T> void iter_accuracy_possibilities(T& fn, std::array<int, word_len> combonation, int iter_number = word_len) {
  if (iter_number == 0) {
    fn(combonation);
  } else {
    for (int i{}; i < 3; i++) {
      combonation[iter_number - 1] = i;
      iter_accuracy_possibilities<word_len>(fn, combonation, iter_number - 1);
    }
  }
}

template <int w_len> float information_eval(std::string guess, const word_set& possible_words) {
  unsigned long total_possibilities;
  float eval;
  std::unordered_set<int> individual_possibilities;
  std::array<int, w_len> cmb;
  
  int times_called{};
  auto evf{[
    &guess,
    &possible_words,
    &total_possibilities,
    &individual_possibilities,
    &times_called
  ](std::array<int, w_len> acc_arr){
    W_word<w_len> ww_with_acc(guess, acc_arr);
    word_set possibilities{ww_with_acc.possibilities(possible_words)};
    unsigned long possibilities_amount{possibilities.size()};
    total_possibilities += possibilities_amount;
    individual_possibilities.insert(possibilities_amount);
  }};

  iter_accuracy_possibilities<w_len>(evf, cmb);

  int iter_n{};
  float tp_as_float{static_cast<float>(total_possibilities)};
  
  for (const int& val: individual_possibilities) {
    if (val) {
      float prob{static_cast<float>(val) / tp_as_float};
      std::cout << (int)(iter_n++) << "| " << val << "| " << prob << " : ";
  
      eval -= prob * std::log2(prob);
      std::cout << eval << '\n';
    } else {
    
    }
  }

  return eval;
}

template <int w_len, int guesses = 6> std::map<float, std::string> w_game_eval(const Wordle_game<w_len, guesses>& w_game, const std::unordered_set<std::string>& possible_words) {
  std::map<float, std::string> eval_to_word;
  for (const std::string& word: possible_words) {
    float eval{information_eval<w_len>(possible_words, word, w_game)};
    eval_to_word.emplace(eval, word);
  }
  return eval_to_word;
}

template <int len, int guesses = 6> void solve_game_io(const std::unordered_set<std::string>& avalible_words) {
  Wordle_game<len, guesses> evaluated_game("");
  while (1) {
    std::map<float, std::string> word_eval{w_game_eval<len, guesses>(evaluated_game, avalible_words)};
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