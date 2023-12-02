#ifndef STEMMING
#define STEMMING

#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <libstemmer.h>
#include <string>
#include <unordered_set>

void stem_word(std::string *word);

bool isStopWord(const std::string &word,
                const std::unordered_set<std::string> &stopWords);

void loadStopWords(std::unordered_map<std::string, int> *stopWords,
                   const std::string &filePath);

#endif