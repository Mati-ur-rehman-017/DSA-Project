
#ifndef INVERTED_INDEX
#define INVERTED_INDEX

#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class node {
public:
  int id;
  int score;
  node *next;
  node(int ID, int Score);
};
class LL {
public:
  node *head;
  node *tail;
  int count;
  LL();
  void insert(int id, int score);
  void clear() {
    node *temp;
    while (head != tail) {
      temp = head;
      head = head->next;
      delete temp;
    }
  }
};
class pairs {
public:
  int id;
  int score;
  pairs(int ID, int Score);
};
class str_pair {
public:
  string title;
  string url;
  int score;
  str_pair(string a, string b, int s);
};
size_t customHash(const std::string &key);
void read_inverted(unordered_map<string, LL, decltype(&customHash)> &,int);
void inverted_index(string a);
void search_title(vector<pairs *> &a, vector<str_pair> &b);
bool check_nullptr(vector<node *> &lists);
void greater_or_equal_tomax(vector<node *> &lists, int max);
int Partition(vector<pairs*> &v, int start, int end);
void Quicksort(vector<pairs*> &v, int start, int end);
void search_words(vector<string>words);

#endif
