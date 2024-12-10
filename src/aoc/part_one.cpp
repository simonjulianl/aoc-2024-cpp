module;

#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_one;

// #define DEBUG

export namespace part_one {

auto solve(const std::string &input) -> long {
  std::deque<std::pair<long, long>> files;
  std::queue<long> emptySpaces;

  long isFile = true, fileId = 0;
  for (auto const &i : input) {
    if (isFile) {
      files.push_back({fileId, i - '0'});
      fileId++;
    } else {
      emptySpaces.push(i - '0');
    }
    isFile = !isFile;
  }

  long long checksum = 0;
  long position = 0;

  bool isCurrentNumber = true;
  while (!files.empty()) {
    if (isCurrentNumber) {
      auto [currentFileId, fileLength] = files.front();
      files.pop_front();

      checksum += currentFileId * ((fileLength * position) +
                                   (fileLength * (fileLength - 1)) / 2);
      position += fileLength;
    } else {
      auto emptySpace = emptySpaces.front();
      emptySpaces.pop();

      // Get the current back to fill the empty spaces
      while (emptySpace > 0) {
        if (files.empty()) {
          break;
        }
        auto [currentFileId, fileLength] = files.back();
        files.pop_back();

        auto current_iteration = std::min(fileLength, emptySpace);

        checksum +=
            currentFileId * ((current_iteration * position) +
                             (current_iteration * (current_iteration - 1)) / 2);
        position += current_iteration;

        emptySpace -= current_iteration;
        fileLength -= current_iteration;

        if (fileLength > 0) {
          files.push_back({currentFileId, fileLength});
        }
      }
    }

    isCurrentNumber = !isCurrentNumber;
  }

  return checksum;
}
} // namespace part_one
