#include <bits/stdc++.h>

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