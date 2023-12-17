#include "inverted_index.hpp"
#include <cstddef>
#include <iterator>
#include <string>
#include <vector>

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

str_pair::str_pair(string a, string b, int s) : title(a), url(b), score(s) {}

// Custom hash function for strings using the djb2 algorithm
size_t customHash(const std::string &key) {
  size_t hash = 5381;

  for (char c : key) {
    hash = ((hash << 5) + hash) + static_cast<size_t>(c); // djb2 hash algorithm
  }

  return hash;
}

void read_inverted(unordered_map<string, LL, decltype(&customHash)> &mp,
                   int barrel) {
  ifstream inverted("inverted/inverted" + to_string(barrel) + ".txt");
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
      mp[word].head = nullptr;
      c = '\\';
    }
    if (c == '\\') {
      id.clear();
      score.clear();
      // Finding whole id
      getline(inverted, id, ':');
      // Finding it's score
      while (inverted.get(c) && c != '\\' && c != '!') {
        score.append(1, c);
      }
      // Adding node to linked list
      mp[word].insert(stoi(id), stoi(score));
    }
  }
  inverted.close();
  return;
}

void writeInverted(unordered_map<string, LL, decltype(&customHash)> &mp,
                   int i) {
  ofstream inverted("inverted/inverted" + to_string(i) + ".txt");
  node *temp;
  for (auto &x : mp) {
    temp = x.second.head;
    inverted << '!' << x.first;
    while (temp != nullptr) {
      inverted << '\\' << temp->id << ':' << temp->score;
      temp = temp->next;
    }
  }
  inverted.close();
}

void inverted_index(string a) {
  vector<unordered_map<string, LL, decltype(&customHash)>> maps(
      100,
      unordered_map<string, LL, decltype(&customHash)>(100000, customHash));
  ifstream forward(a);
  if (!forward.is_open()) {
    cout << "Forward Index not Opening" << endl;
    return;
  }
  string word, id, score;
  char c = forward.get();
  while (!forward.eof()) {
    if (c == '!') {
      if (!id.empty()) {
      }
      id.clear();
      forward.get(c);
      while (c != '\\') {
        id.push_back(c);
        forward.get(c);
      }
    }
    if (c == '\\') {
      word.clear();
      forward.get(c);
      while (c != ':') {
        word.push_back(c);
        forward.get(c);
      }
    }
    if (c == ':') {
      score.clear();
      forward.get(c);
      while (c != '\\' && c != '!' && !forward.eof()) {
        score.push_back(c);
        forward.get(c);
      }
      maps[customHash(word) % 100][word].insert(stoi(id), stoi(score));
    }
  }
  int j = 0;
  for (auto &i : maps) {
    read_inverted(i, j);
    for (auto k : maps[j])
      i.insert(k);
    writeInverted(i, j);
    j++;
  }
}

void search_title(vector<pair<int, int> *> &a, vector<str_pair> &b) {
  ifstream file("ForwardIndex/metadata.txt"),
      file2("ForwardIndex/metadata2.txt");
  if (!file.is_open() || !file2.is_open()) {
    std::cout << "Can Not Open meta data file";
    return;
  }
  // string title, url, id, pos;
  // file2.get();
  // for (int j = 0; j < a.size();) {
  //   getline(file2, id, '\a');
  //   if (a[j]->first == stoi(id)) {
  //     cout<<"a";
  //     getline(file2, pos, '\e');
  //     file.seekg(stoi(pos) + 1);
  //     getline(file, title, '\a');
  //     getline(file, url, '\e');
  //     b[j].title = title;
  //     b[j].url = url;
  //     b[j].score = a[j]->second;
  //     a[j]->first=j;
  //     j++;
  //   } else
  //     file2.ignore(10000000, '\e');
  //   if (file2.eof())
  //     break;
  // }
  // file2.close();
  // file.close();
  // return;
  string  pos;
  char c=file2.get();int k=0;
  for(int j=0;j<a.size();){
    while(k-1!=a[j]->first-1){
      file2.ignore(1000000,'\a');
      k++;
    }
    getline(file2, pos, '\a');
    file.seekg(stoi(pos) + 1);
    getline(file,  b[j].title, '\a');
    getline(file, b[j].url, '\e');
    b[j].score = a[j]->second;
    a[j]->first=j;
    j++;
    k++;
  }
  file2.close();
  file.close();
  return;

}

bool check_nullptr(vector<node *> &lists) {
  for (auto &i : lists)
    if (i == nullptr)
      return true;
  return false;
}

void greater_or_equal_tomax(vector<node *> &lists, int max) {
  for (auto &i : lists)
    while (i != nullptr && i->id < max)
      i = i->next;
}

int Partition(vector<pair<int, int> *> &v, int start, int end) {
  int pivot = end;
  int j = start;
  for (int i = start; i < end; ++i) {
    if (v[i]->second > v[pivot]->second) {
      swap(v[i], v[j]);
      ++j;
    }
  }
  swap(v[j], v[pivot]);
  return j;
}

void Quicksort(vector<pair<int, int> *> &v, int start, int end) {
  if (start < end) {
    int p = Partition(v, start, end);
    Quicksort(v, start, p - 1);
    Quicksort(v, p + 1, end);
  }
}

void search_words(vector<string> words) {

  vector<node *> lists(words.size(), nullptr);
  for (int k = 0; k < words.size(); k++) {
    ifstream inverted("inverted/inverted" +
                      to_string(customHash(words[k]) % 100) + ".txt");
    string word, id;
    int score;
    char c;
    inverted.get();
    node *temp;
    while (!inverted.eof()) {
      getline(inverted, word, '\\');
      if (words[k] == word) {
        c = '\\';
        while (!inverted.eof() && c != '!') {
          score = 0;
          getline(inverted, id, ':');
          while (inverted.get(c) && c != '\\' && c != '!')
            score = score * 10 + (c - 48);
          if (lists[k] == nullptr)
            temp = lists[k] = new node(stoi(id), (score));
          else
            temp = temp->next = new node(stoi(id), (score));
        }
        inverted.close();
        break;
      }
      inverted.ignore(1000000, '!');
      if (inverted.eof()) {
        cout << "words not found";
        inverted.close();
        return;
      }
    }
  }
  int j=0;
  vector<pair<int, int> *> ID;
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
      ID.push_back(new pair<int, int>(lists[0]->id, sum));
      for (int i = 0; i < lists.size(); i++) {
        lists[i] = lists[i]->next;
      }
    }
  }

  if (ID.size() == 0) {
    cout << "No such articles";
    return;
  }
  // Recieving titles and urls in a vector<vector<string>>
  vector<str_pair> info(ID.size(), str_pair("", "", 0));
  search_title(ID, info);
  cout<<"b";
  // Sorting retrieved id's according to score
  Quicksort(ID, 0, ID.size() - 1);
   std::cout << endl << "Total no of articles: " << info.size() << endl;
  for (int i = 0; i < 10; i++) {
    std::cout << "\nArticle " << i + 1 << ":\n";
    cout << info[ID[i]->first].title << "\n" << info[ID[i]->first].url<<info[ID[i]->first].score;
  }
}