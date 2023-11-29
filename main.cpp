#include "src/english_stem.h"
#include <cstring>
#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <map>
#include<chrono>

using namespace std;

//This function takes a string, a separator and a vector. It stores words in vector seperated by the seperator 
void split(const string &s, char seperator, unordered_map<string,int> &words) {
    string word;
        for(int i=0;i<s.size();i++){
            if(isalpha(s[i])){
            word.push_back(s[i]);
            }
            if(isspace(s[i])&&word.length()){
            stemming::stem_word(&word, &word);
            words[word]=words[word]+1;word.clear();
            }
        }
}

//corrected version
void forward_index(string a){
    ifstream file(a+".json");
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << endl;
        return;
    }
    //Parsing json file
    nlohmann::json jsonData;
    file >> jsonData;

    ofstream file_output("index.json");
    if (!file_output.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return;
    }
    file_output<<"[";

    //Accessing each article in json file
    int i=0;
    for (const auto& entry : jsonData) {
        unordered_map<string,int> mp;
        unordered_map<string, int>::iterator it;
        string t = entry["title"];
        string content = entry["content"];
        string url = entry["url"];
        string id=entry["id"];

        //Creating forward index for each index
        split(content,' ',mp);
        nlohmann::json title;title["title"]=t;
        nlohmann::json word;
        for (auto x : mp) {
        word[x.first]=x.second;
        }
        title["words"]=word;
        if(i==0){
            file_output<<title;i++;
        }else{
        file_output<<","<<title;
        }
    }
    file_output << "]"; 
    file_output.close();
    file.close();
}

int main() {
  auto start_time = std::chrono::high_resolution_clock::now();
  forward_index("weareanonymous");
  //read_forward("index");
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << "Time taken by the first part: " << duration.count()/1000000 << "seconds" << std::endl;
  
  return 0;
}
