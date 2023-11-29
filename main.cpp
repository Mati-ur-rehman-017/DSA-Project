#include "nlohmann/json.hpp"
#include "src/english_stem.h"
#include <chrono>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <string.h>
#include <string>

using namespace std;

// This function takes a string, a separator and a vector. It stores words in
// vector seperated by the seperator
void split(const string &s, unordered_map<string, int> &words) {
  string word;
  for (int i = 0; i < s.size(); i++) {
    if (isalpha(s[i])) {
      word.push_back(s[i]);
    }
    if (isspace(s[i]) && word.length()) {
      stemming::stem_word(&word, &word);
      words[word] = words[word] + 1;
      word.clear();
    }
  }
}

// corrected version
void forward_index(string a) {
  ifstream file(a + ".json"), count("count.txt");
  if (!file.is_open() || !count.is_open()) {
    std::cerr << "Failed to open the file." << endl;
    return;
  }
  string number;
  getline(count, number);
  int id = 0;
  char c;
  for (int i = 0; i < number.size(); i++) {
    c = number[i] - 47;
    id = id * 10 + c;
  }
  cout << id;
}
//   // Parsing json file
//   nlohmann::json jsonData;
//   file >> jsonData;

//   ofstream file_output("index.json"), meta_data("metadata.json");
//   if (!file_output.is_open() || !meta_data.is_open()) {
//     std::cerr << "Failed to open the file for writing." << std::endl;
//     return;
//   }
//   file_output << "[";
//   meta_data << "[";
//   // Accessing each article in json file
//   int i = 0;
//   for (const auto &entry : jsonData) {
//     unordered_map<string, int> mp;
//     split(entry["content"], mp);
//     nlohmann::json title, content;
//     content["id"] = title["id"] = i;
//     title["title"] = entry["title"];
//     title["url"] = entry["url"];
//     nlohmann::json word;
//     for (auto x : mp) {
//       word[x.first] = x.second;
//     }
//     content["words"] = word;
//     if (i == 0) {
//       file_output << content;
//       meta_data << title;
//     } else {
//       file_output << "," << content;
//       meta_data << "," << title;
//     }
//     i++;
//   }
//   file_output << "]";
//   meta_data << "]";
//   meta_data.close();
//   file_output.close();
//   file.close();
// }

int main() {
  auto start_time = std::chrono::high_resolution_clock::now();
  forward_index("weareanonymous");
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);

  std::cout << "Time taken by the first part: " << duration.count() / 1000
            << " milliseconds" << std::endl;

  return 0;
}