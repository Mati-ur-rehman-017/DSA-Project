#include "src/forward_index.hpp"

int main() {
  auto start_time = std::chrono::high_resolution_clock::now();
  forwardIndex("NewsData/weareanonymous.json");
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);

  std::cout << "Time taken by the first part: " << duration.count() / 1000
            << " milliseconds" << std::endl;
  return 0;
}