#include "src/forward_index.hpp"
#include "src/inverted_index.hpp"
#include <fstream>
#include <string>
#include <vector>
#include<chrono>



int main() {

  unordered_map<string, LL, decltype(&customHash)> mp(100000, customHash);
  // Make forward and inverted index
  

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
  
  auto start_time = std::chrono::high_resolution_clock::now();
  //read_inverted(mp);  
  //forwardIndex("369news.json");
  //inverted_index("ForwardIndex\\Index.txt");
  search_words(a);
  //cout<<customHash("this")%100;

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);

  std::cout << "\nTime taken by the first part: " << duration.count()/1000
            << " miliseconds" << std::endl;

}
