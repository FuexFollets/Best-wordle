#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <vector>
#include <iterator>

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

template <typename T> map_set<std::string> filter_f(std::ifstream& inp_stream, T fn) {
  map_set<std::string> filtered;
  std::string temp;
  while (std::getline(inp_stream, temp)) {
    if (fn(temp)) { filtered.append(temp); }
  }
  return filtered;
}

template <typename T> std::ostream& operator<< (std::ostream& out, const std::vector<T> vect) {
  out << "[ ";
  for (const T& obj: vect) {
    out << obj << ' ';
  }
  out << ']';
  return out;
}

template <typename T> std::ostream& operator<< (std::ostream& out, const std::set<T> set_obj) {
  out << "{ ";
  for (const T& obj: set_obj) out << obj << ' ';
  out << '}';
}

struct W_letter {
  W_letter(char lett, uint8_t acc) : letter(lett), accuracy(acc) {}
  W_letter () {}
  char letter{32};
  uint8_t accuracy{2};

  friend std::ostream& operator<< (std::ostream& out, W_letter wl) {
    out << (char)wl.letter << ' ' << (int)wl.accuracy;
    return out;
  }
};

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

bool string_count(std::string str, char c) {
  return std::count(str.begin(), str.end(), c);
}

template <typename T> void insert_inc(std::unordered_map<T, int>& map_obj, T obj) {
  if (map_obj.count(obj)) {
    map_obj[obj] = 1;
  } else {
    map_obj[obj]++;
  }
}

template <int len> struct W_word {
  std::string as_std_str;
  std::array<W_letter, len> w_letter_arr;
  bool solved;

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
  }

  W_word() {}

  bool is_correct() {
    for (const auto& c: w_letter_arr) {
      if (!(c.accuracy)) { return false; }
    }
    return true;
  }

  friend std::ostream& operator<< (std::ostream& out, W_word<len> word) {
    for (const auto& wc: word.w_letter_arr) {
      out << wc << " | ";
    }
    return out;
  }

  map_set<std::string> possibilites(std::ifstream& inp_stream) {
    std::unordered_map<int, char> index_is;
    std::unordered_map<int, char> index_is_not;
    std::unordered_set<char> word_not_has;
    std::unordered_map<char, int> word_l_count;
    
    for (int i{}; i < len; i++) {
      W_letter& wl = w_letter_arr[i];
      switch (wl.accuracy) {
        case 0:
          index_is[i] = wl.letter;
          insert_inc(word_l_count, wl.letter);
          break;
        case 1:
          index_is_not[i] = wl.letter;
          insert_inc(word_l_count, wl.letter);
          break;
        case 2:
          word_not_has.insert(wl.letter);
          index_is_not[i] = wl.letter;
          break;
      }
    }

    return filter_f(inp_stream, [&](std::string word){
      word = word.substr(0, len);
      for (const auto&[c, amount]: word_l_count) {
        if (string_count(word, c) <= amount) { return false; }
      }
      
      for (int ind{}; ind < len; ind++) {
        if (index_is.count(ind)) {
          if (index_is.at(ind) != word[ind]) { return false; }
        }
        if (index_is_not.count(ind)) {
          if (index_is_not.at(ind) == word[ind]) { return false; }
        }
      }

      for (const char& c: word_not_has) {
        if (string_count(word, c)) { return false; }
      }
      
      return true;
    });
  }
};

template <int Word_len, int Guesses = 6> class Wordle_game {
  std::array<W_word<Word_len>, Guesses> word_container;
  std::string _solution;
  int appended;

public:
  Wordle_game(std::string solution) : _solution(solution) {}

  void guess_word(std::string word) {
    if (appended == Guesses) { return; }
    word_container[appended++] = W_word<Word_len>(word, _solution);
    solved = solved || word_container[appended].is_correct();
  }

  map_set<std::string> possibilites(std::ifstream& inp_stream) {
    std::unordered_map<int, char> index_is;
    std::unordered_map<int, char> index_is_not;
    std::unordered_set<char> word_not_has;
    std::unordered_map<char, int> word_l_count;

    for (const W_word<Word_len>& w_letter_arr: word_container) {
      for (int i{}; i < Word_len; i++) {
        W_letter& wl = w_letter_arr[i];
        switch (wl.accuracy) {
          case 0:
            index_is[i] = wl.letter;
            insert_inc(word_l_count, wl.letter);
            break;
          case 1:
            index_is_not[i] = wl.letter;
            insert_inc(word_l_count, wl.letter);
            break;
          case 2:
            word_not_has.insert(wl.letter);
            index_is_not[i] = wl.letter;
            break;
        }
      }
    }
    
    return filter_f(inp_stream, [&](std::string word){
      word = word.substr(0, Word_len);
      for (const auto&[c, amount]: word_l_count) {
        if (string_count(word, c) <= amount) { return false; }
      }
      
      for (int ind{}; ind < Word_len; ind++) {
        if (index_is.count(ind)) {
          if (index_is.at(ind) != word[ind]) { return false; }
        }
        if (index_is_not.count(ind)) {
          if (index_is_not.at(ind) == word[ind]) { return false; }
        }
      }

      for (const char& c: word_not_has) {
        if (string_count(word, c)) { return false; }
      }
      
      return true;
    });
  }
};

template <int word_len, int guesses = 6> bool play_game(std::ifstream& inp_stream, std::string solution) {
  Wordle_game<word_len, guesses> game(solution);
  //while ()
  return true;
}

int main() {  
  W_word<4> ww("eerr", "eeer");
  std::cout << ww << '\n';
  
  std::cout << "code compilied!!\n";
}
