#include "updated_primitives.hpp"

constexpr std::array<char*, 3> accuracy_boxes{"🟩", "🟧", "⬛"};

struct W_letter {
  W_letter(char lett, uint8_t acc) : letter(lett), accuracy(acc) {}
  W_letter () {}
  char letter{32};
  uint8_t accuracy{2};

  friend std::ostream& operator<< (std::ostream& out, W_letter wl) {
    out << '[' << (char)wl.letter << accuracy_boxes[(int)wl.accuracy] << ']';
    return out;
  }
};

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

template <int Word_len, int Guesses = 6> class Wordle_game {
  
  std::string _solution;
  int appended{0};
  bool solved{false};

public:
  std::array<W_word<Word_len>, Guesses> word_container;
  Wordle_game(std::string solution) : _solution(solution) {}

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

  std::unordered_set<std::string> possibilities(std::unordered_set<std::string>& inp_words) {
    std::unordered_map<int, char> index_is;
    std::unordered_map<int, std::unordered_set<char>> index_is_not;
    std::unordered_set<char> word_not_has;
    std::unordered_map<char, int> word_l_count;
    std::unordered_set<std::string> possible_words;

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

    
    for (const std::string& word: inp_words) { [&]{
      
      for (const auto&[ind, letter]: index_is) {
        if (word[ind] == letter) { continue; }
        else { return; }
      }

      for (const auto&[ind, not_letters]: index_is_not) {
        if (has_value(not_letters, word[ind])) { return; }
      }

      for (const char& letter: word_not_has) {
        if (string_count(word, letter)) { return; }
      }

      for (const auto&[letter, amount]: word_l_count) {
        if (string_count(word, letter) < amount) { return; }
      }
      
      possible_words.insert(word);
    }(); 
    }

    return possible_words;
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

