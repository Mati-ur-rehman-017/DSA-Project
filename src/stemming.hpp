#ifndef STEMMING
#define STEMMING

#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
<<<<<<< HEAD
#include <libstemmer.h>
=======
//#include <libstemmer.h>
>>>>>>> bb0e7a6b0a06dc45353e739842516cee4312edc9
#include <string>
#include <unordered_set>

void stem_word(std::string *word);

bool isStopWord(const std::string &word,
                const std::unordered_set<std::string> &stopWords);

void loadStopWords(std::unordered_map<std::string, int> *stopWords,
                   const std::string &filePath);

#endif