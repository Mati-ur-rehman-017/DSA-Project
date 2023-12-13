// #include "stemming.hpp"

// void stem_word(std::string *word) {
//   struct sb_stemmer *stemmer = sb_stemmer_new("english", 0);
//   const sb_symbol *stemmed = sb_stemmer_stem(
//       stemmer, reinterpret_cast<const unsigned char *>(word->c_str()),
//       word->length());
//   word->clear();
//   *word = reinterpret_cast<const char *>(stemmed);
// }

// bool isStopWord(const std::string &word,
//                 const std::unordered_set<std::string> &stopWords) {
//   // // Convert both the word and stop words to lowercase
//   // std::string lowercaseWord = word;
//   // std::transform(lowercaseWord.begin(), lowercaseWord.end(),
//   //                lowercaseWord.begin(), ::tolower);

//   // for (const auto &stopWord : stopWords) {
//   //   // Check if the lowercase word matches a stop word
//   //   if (lowercaseWord == stopWord) {
//   //     return true;
//   //   }
//   // }

//   return false;
// }

// void loadStopWords(std::unordered_map<std::string, int> *stopWords,
//                    const std::string &filePath) {}