#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <unordered_map>

template <typename Con> class map_set {
  int appen{0};
  public:
    std::unordered_map<int, Con> as_map;  
    void append(Con obj) { as_map[appen++] = obj; }

    void append_set(map_set<Con> ap_set) {
      for (const auto&[_, obj]: ap_set) { append(obj); }
    }

    inline auto begin() { return as_map.begin(); }
    inline auto end() { return as_map.end(); }
    inline bool empty() { return as_map.empty(); }
    inline void erase(int key) { as_map.erase(key); }
    inline Con at(int n) { return as_map.at(n); }
    
    friend std::ostream& operator<< (std::ostream& out, map_set set) {
      out << "{ ";
      for (const auto&[ind, obj]: set) {
        out << '(' << ind << ": " << obj << "), ";
      }
      out << '}';
      return out;
    }
};

template <typename T> map_set<std::string> filter_f(std::ifstream& inp_stream, T fn) {
  map_set<std::string> filtered;
  std::string temp;
  while (std::getline(inp_stream, temp)) {
    if (fn(temp)) {
      filtered.append(temp);
      //std::cout << temp << ' ';
    }
  }
  return filtered;
}

struct W_letter {
  W_letter(char l, uint8_t acc) : letter{l}, accuracy{acc} {}

  char letter;
  uint8_t accuracy;
};

template <int len> struct W_word {
  W_word(std::string pre, std::string sol) {
    for (int i{0}; i < len; i++) {
      unsigned long int&& loc{sol.find(pre[i])};
      if (loc == i) {
        w_letter_string[i] = W_letter(pre[i], 0);
      } else if (loc < (len + 1)) {
        w_letter_string[i] = W_letter(pre[i], 1);
      } else {
        w_letter_string[i] = W_letter(pre[i], 2);
      }
    }
  }
  std::array<W_letter, len> w_letter_string;
};

template <int Word_len, typename T> struct Wordle_game {
  Wordle_game(T word) : solution{word} {}

  std::array<W_word<Word_len>, 6> guessed;
  uint8_t appended{};
  
  std::string solution;

  template <typename Wt> void guess_word(Wt word) {
    
  }
};

int main() {
  std::string words_pathname;
  std::getline(std::cin, words_pathname);
  std::ifstream a_stream(words_pathname);

  auto all_words{filter_f(a_stream, [](auto inp){
    return true;
  })};

  std::cout << "the 4007th word is " << all_words.at(4007) << '\n';
  
  std::cout << "Hello World!\n";
}