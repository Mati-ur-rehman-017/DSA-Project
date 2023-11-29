#include "src/english_stem.h"
#include <cstring>
#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <map>

using namespace std;

//This function takes a string, a separator and a vector. It stores words in vector seperated by the seperator 
void split(const string &s, char seperator, unordered_map<string,int> &words) {
    string word;
        for(int i=0;i<s.size();i++){
            if(isalpha(s[i])){
            word.push_back(s[i]);
            }
            if(isspace(s[i])&&word.length()){
            words[word]=words[word]+1;word.clear();
            }
        }
}


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
    unordered_map<string,int> mp;
    unordered_map<string, int>::iterator it;
    
    nlohmann::json jsonArray = nlohmann::json::array();
    //Accessing each article in json file
    for (const auto& entry : jsonData) {
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
        jsonArray.push_back(title);
    }
    file_output<< std::setw(4) << jsonArray << std::endl;
    file_output.close();
    file.close();
}

int main() {
  // string word = "Underrated";
  // stemming::stem_word(&word, &word);
  // cout << word;
  forward_index("weareanonymous");

  return 0;
}
