#include <bits/stdc++.h>

typedef std::unordered_set<std::string> word_set;

bool string_count(std::string str, char c) {
  return std::count(str.begin(), str.end(), c);
}

template <typename T, typename U> bool has_key(std::unordered_map<T, U> map, T key) {
  return map.find(key) != map.end();
}

template <typename T> bool has_value(std::unordered_set<T> set, T value) {
  return set.find(value) != set.end();
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

template <int len> word_set words_from_file(std::ifstream& inp_filestream) {
  word_set word_collection;
  std::string temp;

  while (std::getline(inp_filestream, temp)) word_collection.insert(temp);

  return word_collection;
}

bool valid_word(std::string word, word_set& avalible_words) { return has_value(avalible_words, word); }

template <typename K, typename V> std::ostream& operator<< (std::ostream& out, std::unordered_map<K, V> m) {
  out << "{ ";
  for (const auto&[key, value]: m) out << key << " : " << value << ", ";
  out << "}";
  return out;
}

template <typename T> std::ostream& operator<< (std::ostream& out, std::unordered_set<T> s) {
  out << "{ ";
  for (const T& val: s) out << val << ", ";
  out << "}";
  return out;
}