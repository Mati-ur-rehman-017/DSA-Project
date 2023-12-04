#include "src/forward_index.hpp"
#include "src/inverted_index.hpp"
#include <filesystem>



int main()
{

  unordered_map<string, LL, decltype(&customHash)> mp(100000, customHash);//read_inverted(mp);
  read_inverted(mp);
  

  //Make forward and inverted index  
  // forwardIndex("cnn.json");
  // inverted_index("ForwardIndex\\Index.txt",mp);

  vector<string>a;
  cout<<"Enter your Query : ";
  string t;
    getline(cin,t);

    istringstream iss(t);
    string word;
    while(iss >> word) {
        transform(word.begin(), word.end(), word.begin(),[](unsigned char c) { return std::tolower(c); });
        a.push_back(word);
    }
  auto start_time = std::chrono::high_resolution_clock::now();
  search_words(a,mp);
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << "\nTime taken by the first part: " << duration.count() / 1000 << "miliseconds" << std::endl;
}
