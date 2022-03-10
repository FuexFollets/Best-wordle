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

template <int w_len> float information_eval(const std::string guess, const word_set& possible_words) {
  size_t total_possibilities;
  float eval;
  std::unordered_set<size_t> individual_possibilities;
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
    size_t possibilities_amount{possibilities.size()};
    total_possibilities += possibilities_amount;
    individual_possibilities.insert(possibilities_amount);
  }};

  iter_accuracy_possibilities<w_len>(evf, cmb);

  unsigned long iter_n{};
  float tp_as_float{static_cast<float>(total_possibilities)};
  
  for (const unsigned long& val: individual_possibilities) {
    if (val) {
      float prob{static_cast<float>(val) / tp_as_float};
      std::cout << (int)(iter_n++) << "| " << val << "| " << prob << '\n';
  
      eval -= prob * std::log2(prob);
      //std::cout << eval << '\n';
    }
  }
  return eval;
}

template <int w_len> std::map<float, std::string> ww_all_eval(const word_set& possible_words, unsigned long* progress = nullptr) {
  std::map<float, std::string> eval_to_word;
  for (const std::string& word: possible_words) {
    float w_eval{information_eval<w_len>(word, possible_words)};
    std::cout << "Evaluated " << word << " to " << w_eval << std::endl;
    eval_to_word.emplace(w_eval, word);
  }
  return eval_to_word;
}

template <int len, int guesses = 6> void solve_game_io(const word_set& avalible_words) {
  Wordle_game<len, guesses> displayed("");
  std::array<word_set, guesses> updated_av_words;
  updated_av_words[0] = avalible_words;
  int avw_int{};
  
  while (1) {
    std::map<float, std::string> words_eval{ww_all_eval<len>(updated_av_words[avw_int])};
    int iter_times{std::min(words_eval.size(), (unsigned long)5)};
    auto itr{words_eval.end()};
    
    for (int i{}; i < iter_times; i++) {
      auto&[ev, word]{*(--itr)};
      std::cout << word << " | " << ev << '\n';
    }

    std::cout << displayed;
    W_word<len> res{w_word_io<len>()};

    displayed.append_w_word(res);
    updated_av_words[avw_int + 1] = res.possibilities(updated_av_words[avw_int]);
    avw_int++;
  }
}