#include "forward_index.hpp"
#include <fstream>
#include <string>

void split(const string &s, unordered_map<string, int> &words) {
  string word;
  for (int i = 0; i < s.size(); i++) {
    if (isalpha(s[i])) {
      word.push_back(tolower(s[i]));
    }
    if (isspace(s[i]) && word.length()) {
      words[word]++;
      word.clear();
    }
  }
}
void split(string &s, unordered_map<string, int> &words) {
  string word, word2;
  for (int i = 0; i < s.size(); i++) {
    if (isalpha(s[i]) || isspace(s[i])) {
      if (isalpha(s[i])) {
        word2.push_back(tolower(s[i]));
      }
      if (isspace(s[i]) && word.length()) {
        words[word2] = words[word2] + 10;
        word2.clear();
      }
      word.push_back(s[i]);
    }
  }
  s = word;
}
int getID() {
  ifstream count("ForwardIndex/count.txt", ios::app);
  if (!count.is_open()) {
    cout << "CanNot Open file Count";
    exit(EXIT_FAILURE);
  }
  string number;
  getline(count, number);
  int id = 0;
  for (int i = 0; i < number.size(); i++)
    id = id * 10 + (number[i] - 48);
  count.close();
  return id;
}

void forwardIndex(const string &path) {
  // Opening File to Read
  int id = getID();
  ifstream file("NewsData/" + path);
  if (!file.is_open()) {
    std::cerr << "Failed to open the file." << endl;
    return;
  }

  // Parsing json file
  nlohmann::json jsonData;
  file >> jsonData;

  // Opening Files to write forward index and metadata
  ofstream file_output("ForwardIndex/Index.txt"),
      meta_data("ForwardIndex/metadata.txt", ios_base::app),
      meta_data2("ForwardIndex/metadata2.txt", ios_base::app);
  if (!file_output.is_open() || !meta_data.is_open()) {
    std::cerr << "Failed to open the file for writing." << std::endl;
    return;
  }

  // Accessing each article in json file
  unordered_map<string, int> mp;
  for (const auto &entry : jsonData) {
    string title = entry["title"], url = entry["url"];
    split(entry["content"], mp);
    split(title, mp);
    meta_data2 << '\e' << id << '\a' << meta_data.tellp();
    meta_data << '\e' << title << '\a' << entry["url"];
    file_output << '!' << id;
    for (auto x : mp) {
      file_output << '\\' << x.first << ':' << x.second;
    }
    id++;
    mp.clear();
  }
  meta_data.close();
  file_output.close();
  meta_data2.close();
  file.close();
  ofstream count_out("ForwardIndex/count.txt");
  count_out<<id;
}
