#include "forward_index.hpp"

void stem_word(std::string *word) {
  std::wstring w_word(word->begin(), word->end());
  stemming::english_stem<std::wstring> Stem;
  Stem(w_word);
  word->clear();
  for (int i = 0; i < w_word.size(); i++)
    word->push_back(w_word[i]);
}

void split(const string &s, unordered_map<string, int> &words) {
  string word;
  for (int i = 0; i < s.size(); i++) {
    if (isalpha(s[i])) {
      word.push_back(s[i]);
    }
    if (isspace(s[i]) && word.length()) {
      // stemming::stem_word(&word, &word);
      words[word] = words[word] + 1;
      word.clear();
    }
  }
}

int getID() {
  ifstream count("ForwardIndex/count.txt");
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

void forwardIndex(const string &filePath) {
  // Opening File to Read
  ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Failed to open the file." << endl;
    return;
  }

  // Parsing json file
  nlohmann::json jsonData;
  file >> jsonData;

  // Getting Id For giving to documents
  int id = getID();

  // Opening Files to write forward index and metadata
  ofstream file_output("ForwardIndex/index.txt", ios_base::app),
      meta_data("ForwardIndex/metadata.txt", ios_base::app);
  if (!file_output.is_open() || !meta_data.is_open()) {
    std::cerr << "Failed to open the file for writing." << std::endl;
    return;
  }

  // Accessing each article in json file
  unordered_map<string, int> mp;
  for (const auto &entry : jsonData) {
    split(entry["content"], mp);
    meta_data << '!' << id << '\\' << entry["title"] << ':' << entry["url"];
    file_output << '!' << id;
    for (auto x : mp) {
      file_output << '\\' << x.first << ':' << x.second;
    }
    id++;
    mp.clear();
  }
  meta_data.close();
  file_output.close();
  file.close();
  ofstream count_out("ForwardIndex/count.txt");
  count_out << id;
}