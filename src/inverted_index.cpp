#include "inverted_index.hpp"
#include <bits\\algorithmfwd.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <ios>
#include <iostream>
#include <iterator>
#include <ratio>
#include <string>
#include <unordered_map>

node::node(int ID, int Score) : id(ID), score(Score), next(nullptr) {}

LL::LL() : head(nullptr), tail(nullptr), count(0) {}

void LL::insert(int id, int score) {
  count++;
  if (tail == nullptr) {
    tail = head = new node(id, score);
    return;
  }
  tail->next = new node(id, score);
  tail = tail->next;
}

pairs::pairs(int ID, int Score) : id(ID), score(Score) {}
str_pair::str_pair(string a, string b, int s) : title(a), url(b), score(s) {}

// Custom hash function for strings using the djb2 algorithm
size_t customHash(const std::string &key) {
  size_t hash = 5381;

  for (char c : key) {
    hash = ((hash << 5) + hash) + static_cast<size_t>(c); // djb2 hash algorithm
  }

  return hash;
}


void read_inverted(unordered_map<string, LL, decltype(&customHash)> &mp,int barrel) {
  ifstream inverted("inverted\\inverted"+to_string(barrel)+".txt");
  if (!inverted.is_open()) {
    return;
  }
  char c;
  string word, id, score;
  c = inverted.get();
  while (!inverted.eof()) {
    // New Word Detected
    // inverted.ignore(1000, '!');
    if (c == '!') {
    word.clear();
    getline(inverted, word, '\\');
    mp[word].head = nullptr;c='\\';
    }
    if (c == '\\') {
    id.clear();
    score.clear();
    // Finding whole id
    getline(inverted, id, ':');
    // Finding it's score
    while (inverted.get(c)&& c!= '\\' && c != '!') {
      score.append(1,c);
    }
    // Adding node to linked list
    mp[word].insert(stoi(id), stoi(score));
    }
  }
  inverted.close();
  return;
}

void inverted_index(string a) {
  int barrels=100;
  for(int k=0;k<barrels;k++){
  unordered_map<string, LL, decltype(&customHash)> mp(100000, customHash);
  read_inverted(mp,k);
  ifstream forward(a);
  if (!forward.is_open()) {
    cout << "Can Not Open Forward index file";
    return;
  }
  char c = forward.get();
  string id, word, score;
  while (c != EOF) {
    if (c == '!') {
      id.clear();
      // getting full id
      while ((c = forward.get()) != '\\') {
        id.append(1,c);
      }
    }
    // Word detection
    if (c == '\\') {
      word.clear();
      while ((c = forward.get()) != ':') {
        word.append(1,c);
      }
      score.clear();
      while ((c = forward.get()) != '\\' && c != '!' && c != EOF){
        score.append(1,c);
      }
      if(customHash(word)%barrels==k){
        mp[word].insert(stoi(id), stoi(score));
      }
    }
  }
  forward.close();
  // Writing to inverted.txt file whole inverted index
  ofstream inverted("inverted\\inverted"+to_string(k)+".txt");
  ofstream inverted_meta("inverted\\inverted_meta"+to_string(k)+".txt");
  node *temp;
  for (auto &x : mp) {
    inverted << '!' << x.first;
    std::streampos pos=inverted.tellp();
    inverted_meta<<'!'<<x.first<<"#"<<pos;
    temp = x.second.head;
    while (temp != nullptr) {
      inverted << '\\' << temp->id << ':' << temp->score;
      temp = temp->next;
    }
  }
  inverted.close();
  inverted_meta.close();
  }
}

void search_title(vector<pairs *> &a, vector<str_pair> &b) {
  ifstream file("ForwardIndex\\metadata.txt"),file2("ForwardIndex\\metadata2.txt");
  if (!file.is_open()) {
    std::cout << "Can Not Open meta data file";
    return;
  }
  str_pair d("", "", 0);
  for (int i = 0; i < a.size(); i++) {
    b.push_back(d);
  }
  // int article_id = a[0]->id, id = 0, jump = 0;
  // char c = file.get();
  // string title, url;
  // int j = 0;
  // while (!file.eof()) {
  //   if (c == '`') {
  //     id = 0;
  //     while (file.get(c) && c != '\e' && !file.eof()) {
  //       id = id * 10 + (c - 48);
  //       // cout << id << endl;
  //     }
  //   }
  //   if (article_id == id) {
  //     file.ignore(10000, '\\');
  //     title.clear();
  //     while (file.get(c) && c != ':') {
  //       title.append(1,c);
  //     }
  //     url.clear();
  //     while (file.get(c) && c != '`') {
  //       url.append(1,c);
  //     }
  //     b[j].title = title;
  //     b[j].url = url;
  //     b[j].score = a[j]->score;
  //     j++;
  //     if (j == a.size()) {
  //       return;
  //     }
  //     article_id = a[j]->id;
  //   } else {
  //     file.ignore(1000000,'`');
  //     if(!file.eof())
  //       c='`';
  //     else 
  //     break;
  //   }
  // }
   int article_id = a[0]->id;
  char c = file.get();
  string title, url,id,pos;
  int j = 0;file2.get();
  while(1){
    getline(file2,id,'#');
    pos.clear();
    while(file2.get(c)&&c!='!'){
      pos.append(1,c);
    }
    if(article_id==stoi(id)){
      title.clear();
      getline(file,title,':');
      url.clear();
      while (file.get(c) && c != '`') {
        url.append(1,c);
      }
      b[j].title = title;
      b[j].url = url;
      b[j].score = a[j]->score;
      j++;
      if (j == a.size()) {
        file2.close();
        file.close();return;
      }
      article_id = a[j]->id;
    }
  }
  
}

bool check_nullptr(vector<node *> &lists) {
  for (int i = 0; i < lists.size(); i++) {
    if (lists[i] == nullptr) {
      return true;
    }
  }
  return false;
}

void greater_or_equal_tomax(vector<node *> &lists, int max) {
  for (int i = 0; i < lists.size(); i++) {
    while (lists[i] != nullptr && lists[i]->id < max) {
      lists[i] = lists[i]->next;
    }
  }
}

int Partition(vector<pairs*> &v, int start, int end) {
  int pivot = end;
  int j = start;
  for (int i = start; i < end; ++i) {
    if (v[i]->score > v[pivot]->score) {
      swap(v[i], v[j]);
      ++j;
    }
  }
  swap(v[j], v[pivot]);
  return j;
}

void Quicksort(vector<pairs*> &v, int start, int end) {
  if (start < end) {
    int p = Partition(v, start, end);
    Quicksort(v, start, p - 1);
    Quicksort(v, p + 1, end);
  }
}

void search_words(vector<string> words) {

  vector<node *> lists;
  for(int i=0;i<words.size();i++){
    lists.push_back(nullptr);
  }
  // ifstream inverted("inverted.txt");node* temp;node*temp2;
  // char c=inverted.get();
  // string word,id,score;
  // int j=0,check=0;
  // while(!inverted.eof()){
  //   if(c=='!'){
  //     getline(inverted,word,'\\');
  //     for(int i=0;i<words.size();i++){
  //       if(words[i]==word){
  //         check=1;
  //         temp=lists[i];
  //         c='\\';
  //         while(!inverted.eof()&&c!='!'){
  //         id.clear();score.clear();
  //         getline(inverted,id,':');
  //          while(inverted.get(c)&&c!='\\'&&c!='!'){
  //           score.append(1,c);
  //         }
  //         temp2=new node(stoi(id),stoi(score));
  //         if(lists[i]==nullptr){
  //           lists[i]=temp2;temp=temp2;
  //         }else{
  //           temp->next=temp2;
  //           temp=temp2;
  //         }
  //         }
  //       }
  //       j++;
  //       if(j==words.size()){
  //         break;
  //       }
  //     }
  //     if(check==0){
  //       inverted.ignore(1000000,'!');
  //     }else{
  //       check=0;
  //     }
  //   }
  // }
  // inverted.close();
  for(int k=0;k<words.size();k++){

  ifstream inverted("inverted\\inverted"+to_string(customHash(words[k])%100)+".txt");
  ifstream inverted_meta("inverted\\inverted_meta"+to_string(customHash(words[k])%100)+".txt");
  string word,pos,id,score;
  char c;
  inverted_meta.get();node* temp;node*temp2;
  while(!inverted_meta.eof()){
    word.clear();
    getline(inverted_meta,word,'#');

      if(words[k]==word){
        pos.clear();
        while(inverted_meta.get(c)&&c!='!'){
          pos.append(1,c);
        }
        inverted.seekg(stoi(pos)+1);
        temp=lists[k];
        c='\\';
        while(!inverted.eof()&&c!='!'){
        id.clear();score.clear();
        getline(inverted,id,':');
        while(inverted.get(c)&&c!='\\'&&c!='!'){
          score.append(1,c);
        }
        temp2=new node(stoi(id),stoi(score));
        if(lists[k]==nullptr){
          lists[k]=temp2;temp=temp2;
        }else{
          temp->next=temp2;
          temp=temp2;
        }
      }
      inverted.close();inverted_meta.close();break;
    }else{
      while(inverted_meta.get(c)&&c!='!'){}
    }
    if(inverted_meta.eof()){
    cout<<"words not found";inverted.close();inverted_meta.close();return;
    }
  }
}

  vector<pairs *> ID;
  if(lists.size()==1){
    while(lists[0]!=nullptr){
     ID.push_back(new pairs(lists[0]->id,lists[0]->score));
    lists[0]=lists[0]->next;
    }
  }else{
    while (!check_nullptr(lists)) {
    int id = lists[0]->id, max = -1;
    for (int i = 0; i < lists.size(); i++) {
      if (id != lists[i]->id) {
        for (; i < lists.size(); i++) {
          if (id < lists[i]->id) {
            id = lists[i]->id;
            max = id;
          } else if (max == -1) {
            max = id;
          }
        }
      }
    }
    if (max != -1) {
      greater_or_equal_tomax(lists, max);
    } else {
      int sum = 0;
      for (int i = 0; i < lists.size(); i++) {
        sum += lists[i]->score;
      }
      ID.push_back(new pairs(lists[0]->id, sum));
      for (int i = 0; i < lists.size(); i++) {
        lists[i] = lists[i]->next;
      }
    }
  }
}
  if (ID.size() == 0) {
    cout << "No such articles";
    return;
  }
  // Recieving titles and urls in a vector<vector<string>>
  vector<str_pair> info;
  search_title(ID, info);
  // Sorting retrieved id's according to score
  Quicksort(ID, 0, ID.size() - 1);
  std::cout << endl << "Total no of articles: " << ID.size() << endl;
  // for (int i = 0; i < info.size(); i++) {
  //   std::cout << "\nArticle " << i + 1 << ":\n";
  //   cout << info[i].title << "\n" << info[i].url;// }
}
