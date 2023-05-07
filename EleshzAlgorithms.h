#include <string>
#include <filesystem>

namespace fs = std::filesystem;

fs::path QuickSearch(const fs::path StartSearchLocation, const fs::path FileToBeFound, const int ThreadsToStart);
