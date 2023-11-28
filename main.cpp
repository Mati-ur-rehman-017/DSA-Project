#include "src/english_stem.h"
#include <cstring>
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

int main() {
  string word = "Underrated";
  stemming::stem_word(&word, &word);
  cout << word;
  return 0;
}
