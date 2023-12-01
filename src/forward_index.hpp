#ifndef FORWARD_INDEX
#define FORWARD_INDEX

#include "nlohmann/json.hpp"
#include "stemming/english_stem.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

void stem_word(std::string *);
void split(const string &, unordered_map<string, int> &);
int getID();
void forwardIndex(const string &);

#endif