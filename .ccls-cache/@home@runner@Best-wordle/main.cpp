#include <bits/stdc++.h>
#include "wordle_objects.hpp"
#include "evaluation.hpp"
////
/*
template <typename T> class map_set {
  int appen{0};
  public:
    std::unordered_map<int, T> as_map;  
    void append(T obj) { as_map[appen++] = obj; }

    void append_set(map_set<T> ap_set) {
      for (const auto&[_, obj]: ap_set) { append(obj); }
    }

    inline auto begin() { return as_map.begin(); }
    inline auto end() { return as_map.end(); }
    inline bool empty() { return as_map.empty(); }
    inline void erase(int key) { as_map.erase(key); }
    inline T at(int n) { return as_map.at(n); }
    
    friend std::ostream& operator<< (std::ostream& out, map_set set) {
      out << "{ ";
      for (const auto&[ind, obj]: set) {
        out << '(' << ind << ": " << obj << "), ";
      }
      out << '}';
      return out;
    }
};

template <typename T, typename U>
bool has_key(std::unordered_map<T, U> map, T key) {
  return map.find(key) != map.end();
}


template <typename T>
bool has_value(std::unordered_set<T> set, T value) {
  return set.find(value) != set.end();
}

//
// File Filter Functions
//

template <typename T> std::unordered_set<std::string> filter_f(std::ifstream& inp_stream, T fn) {
  std::unordered_set<std::string> filtered;
  std::string temp;
  while (std::getline(inp_stream, temp)) {
    if (fn(temp)) { filtered.insert(temp); }
  }
  return filtered;
}

template <typename T> void iter_file_by_line(std::ifstream& inp_stream, T fn) {
  std::string temp;
  while (std::getline(inp_stream, temp)) {
    fn(temp);
  }
}

bool valid_word(std::ifstream& word_file, std::string tested_word) {
  std::string temp;
  while (std::getline(word_file, temp)) {
    if (tested_word == temp) {
      return true;
    }
  }
  return false;
}

template <typename T> std::ostream& operator<< (std::ostream& out, std::unordered_set<T> container) {
  out << "{ ";
  for (const T& val: container) out << val << ", ";
  out << '}';
  return out;
}


template <typename T, typename U> std::ostream& operator<< (std::ostream& out, std::unordered_map<T, U> container) {
  out << "{ ";
  for (const auto&[key, val]: container) out << key << ": " << val << ", ";
  out << "}";
  return out;
}
//
// Word Intersection logical functions
//

std::string common(std::string s1, std::string s2) {
  std::string common_chars;
  auto len{s1.length()};
  for (const char& c: s1) {
    auto loc{s2.find(c)};
    if (loc < len) {
      common_chars.push_back(c);
      s2.erase(loc, 1);
    }
  }
  return common_chars;
}

std::array<std::string, 2> without(std::string s1, std::string s2) {
  unsigned long int len{s1.length()};
  std::string common_chars{common(s1, s2)};

  for (const char& c: common_chars) {
    s1.erase(s1.find(c), 1);
    s2.erase(s2.find(c), 1);
  }
  
  return std::array<std::string, 2>({s1, s2});
}
 

//
// Wordle Structre Elements
//
/*
struct W_letter {
  W_letter(char lett, uint8_t acc) : letter(lett), accuracy(acc) {}
  W_letter () {}
  char letter{32};
  uint8_t accuracy{2};

  friend std::ostream& operator<< (std::ostream& out, W_letter wl) {
    out << '[' << (char)wl.letter << accuracy_boxes[(int)wl.accuracy] /*<< (int)wl.accuracy << ']';
    return out;
  }
};

bool string_count(std::string str, char c) {
  return std::count(str.begin(), str.end(), c);
}

template <int len> struct W_word {
  std::string as_std_str;
  std::array<W_letter, len> w_letter_arr;
  bool solved{true};
  bool valid{false};

  W_word(std::string word, std::string sol) : as_std_str(word) {
    std::string word_cpy{word};
    std::string correct_l;
    std::set<int> _0acc;
    std::vector<int> not_0acc;
    
    for (int i{}; i < len; i++) {
      if (word[i] == sol[i]) {
        w_letter_arr[i] = W_letter(word[i], 0);
        _0acc.insert(i);
      } else { not_0acc.push_back(i); }
    }
    for (int ind{len}; ind > -1; ind--) {
      if (_0acc.find(ind) != _0acc.end()) {
        word.erase(ind, 1); sol.erase(ind, 1);
      }
    }
    
    std::string common_l{common(word, sol)};
    unsigned long common_l_len{common_l.length()};
    
    for (const char& c: common_l) {
      w_letter_arr[not_0acc[word.find(c)]] = W_letter(c, 1);
    }
    for (int i{}; i < len; i++) {
      if (w_letter_arr[i].accuracy == 2) {
        w_letter_arr[i] = W_letter(word_cpy[i], 2);
      }
    }

    for (const W_letter& wl: w_letter_arr) {
      solved = solved && !wl.accuracy;
    }

    
  }

  W_word(std::string word_as_str, std::array<int, len> acc_arr) : as_std_str(word_as_str) {
    int ind{};
    for (W_letter& wl: w_letter_arr) {
      wl = W_letter(word_as_str[ind], acc_arr[ind]);
      ind++;
    }
  }

  W_word() {}

  friend std::ostream& operator<< (std::ostream& out, W_word<len> word) {
    for (const auto& wc: word.w_letter_arr) {
      out << wc;
    }
    return out;
  }
};

template <int w_len> W_word<w_len> w_word_io() {
  std::string inp;
  std::array<int, w_len> acc_arr;
  std::cout << "Input W_word as <word string><integer accuracies>: ";
  std::getline(std::cin, inp);
  std::string word_str{inp.substr(0, w_len)};
  for (int ind{}; ind < w_len; ind++) {
    acc_arr[ind] = inp[w_len + ind] - '0';
  }
  return W_word<w_len>(word_str, acc_arr);
}
/*
template <int Word_len, int Guesses = 6> class Wordle_game {
  
  std::string _solution;
  int appended{0};
  bool solved{false};

public:
  std::array<W_word<Word_len>, Guesses> word_container;
  Wordle_game(std::string solution) : _solution(solution) {}

  static Wordle_game<Word_len> blank_g() {
    return Wordle_game<Word_len>("");
  }

  void guess_word(std::string word) {
    if (appended == Guesses) { return; }
    word_container[appended++] = W_word<Word_len>(word, _solution);
    solved = solved || word_container[appended].solved;
  }

  void append_w_word(const W_word<Word_len> word) {
    if (appended == Guesses) { return; }
    word_container[appended++] = word;
    solved = solved || word_container[appended].solved;
  }


  std::unordered_set<std::string> possibilities(std::ifstream& inp_stream) {
    std::unordered_map<int, char> index_is;
    std::unordered_map<int, std::unordered_set<char>> index_is_not;
    std::unordered_set<char> word_not_has;
    std::unordered_map<char, int> word_l_count;
    
    int iteration_number{0};
    for (const W_word<Word_len>& word: word_container) {
      std::unordered_map<char, int> lett_count;
      const std::string& word_as_str{word.as_std_str};
      
      if (iteration_number++ == appended) { break; }
      
      for (int i{}; i < Word_len; i++) {
        const W_letter& wl{word.w_letter_arr[i]};
        const char& letter{wl.letter};
        switch (wl.accuracy) {
          case 2:
            index_is_not[i].insert(letter);
            word_not_has.insert(letter);
            break;
          case 1:
            index_is_not[i].insert(letter);
            lett_count[letter]++;
            break;
          case 0:
            index_is[i] = letter;
            lett_count[letter]++;
        }
      }
      
      for (const auto&[letter, amount]: lett_count) {
        word_l_count[letter] = std::max(word_l_count[letter], amount);
      }
    }
    std::unordered_set<char> to_erase;
    for (const char& letter: word_not_has) {
      if (has_key(word_l_count, letter)) {
        to_erase.insert(letter);
      }
    }
    for (const char& letter: to_erase) { word_not_has.erase(letter); }

    return filter_f(inp_stream, [
      &index_is,
      &index_is_not,
      &word_l_count,
      &word_not_has](std::string word){
      word = word.substr(0, Word_len);
      
      for (const auto&[ind, letter]: index_is) {
        if (word[ind] == letter) { continue; }
        else { return false; }
      }

      for (const auto&[ind, not_letters]: index_is_not) {
        if (has_value(not_letters, word[ind])) { return false; }
      }

      for (const char& letter: word_not_has) {
        if (string_count(word, letter)) { return false; }
      }

      for (const auto&[letter, amount]: word_l_count) {
        if (string_count(word, letter) < amount) { return false; }
      }
      
      return true;
    }); 
    
  }

  bool is_full() {
    return (bool)(word_container[Guesses].as_std_str[0] - 32);
  }

  bool is_over() {
    return (solved || (appended == Guesses));
  }

  template <int WL, int G> friend std::ostream& operator<< (std::ostream& out, Wordle_game<WL, G> w_game) {
    for (const auto& word: w_game.word_container) {
      out << word << '\n';
    }
    return out;
  }
};

template <int word_len, int guesses = 6> bool play_game(std::ifstream& inp_stream, std::string solution) {
  std::cout << "welcome to wordle!\n";
  Wordle_game<word_len, guesses> game(solution);
  while (!game.is_over()) {
    std::cout << "guess a word: ";
    std::string guess;
    std::getline(std::cin, guess);
    game.guess_word(guess);
    std::cout << game;
  }
}

/*
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
  ](std::array<int, w_len> p_acc_arr){
    auto w_game_cpy{W_game};
    w_game_cpy.append_w_word(W_word<w_len>(guess, p_acc_arr));
    int possibilities{static_cast<int>(w_game_cpy.possibilities(word_file).size())};
    total_possibilities += possibilities;
    acc_arr_to_combonations.append(std::pair<std::array<int, w_len>, int>(p_acc_arr, possibilities));
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
*/
//

int main() {
  std::ifstream word_file{"5lw.txt"};

  Wordle_game<5> wg("zzzzz");

  wg.append_word();
  //float infor{information_eval<5>("crate", wg, word_file)};
  
  //std::cout << infor << '\n';

  
  
  std::cout << '\n' << "Code compilied" << '\n';  
  return 0;
}
