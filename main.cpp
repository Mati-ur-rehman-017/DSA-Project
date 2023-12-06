#include "src\\forward_index.hpp"
#include "src\\inverted_index.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

int main() {

  unordered_map<string, int, decltype(&customHash)> mp(100000, customHash);
  //  read_inverted(mp);

  // Make forward and inverted index
  // forwardIndex("cnn.json");
  //  inverted_index("ForwardIndex\\Index.txt",mp);

  vector<string> a;
  cout << "Enter your Query : ";
  string t;
  getline(cin, t);

  istringstream iss(t);
  string word;
  while (iss >> word) {
    transform(word.begin(), word.end(), word.begin(),
              [](unsigned char c) { return std::tolower(c); });
    a.push_back(word);
  }

  // string line;
  auto start_time = std::chrono::high_resolution_clock::now();
  // forwardIndex("cnn.json");
  // inverted_index("ForwardIndex/Index.txt", mp);
  // search_title(a, results);
  // cout << 0;
  // // cout << 1;
    // ivert.seekg(103244381);
  read_inverted(mp);
  search_words(a, mp);
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  // cout << results[0].title << endl << results[0].url << endl;

  std::cout << "\nTime taken by the first part: " << duration.count() / 1000
            << " miliseconds" << std::endl;
  // ivert.close();
}
