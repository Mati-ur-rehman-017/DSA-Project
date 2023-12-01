#include "src/forward_index.hpp"
#include <filesystem>

int main() {
  const std::string folderPath = "NewsData/";

  try {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
      if (entry.is_regular_file() && entry.path().extension() == ".json") {
        forwardIndex(entry.path().filename());
      }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    std::cout << "Time taken For the whole DataSet: "
              << duration.count() / 60000000 << " Minutes" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
