module;

#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_two;

// #define DEBUG

export namespace part_two {

auto solve(const std::string &input) -> long {
  std::list<std::pair<long, long>> layout;

  long isFile = true, fileId = 0;
  for (auto const &i : input) {
    if (isFile) {
      layout.push_back({fileId, i - '0'});
      fileId++;
    } else {
      layout.push_back({-1, i - '0'});
    }
    isFile = !isFile;
  }

  std::unordered_set<long> seen_files;

  // defragmentation
  for (auto it = layout.rbegin(); it != layout.rend(); it++) {
    auto [currentFileId, fileLength] = *it;
    if (currentFileId == -1) {
      continue; // layout for space, just ignore it
    }

    if (seen_files.find(currentFileId) != seen_files.end()) {
      continue; // already seen this file
    }

    // attempt to move this
    seen_files.insert(currentFileId);

    // Try to insert from front
    for (auto it2 = layout.begin(); it2 != layout.end(); it2++) {
      if (*it == *it2) { // do not try to move the file backwards
        break;
      }

      auto [currentFileId2, fileLength2] = *it2;

      // empty space
      if (currentFileId2 == -1) {
        // the block can be inserted here
        if (fileLength2 >= fileLength) {
          layout.insert(it2, {currentFileId, fileLength});
          it2->second -= fileLength;
          it->first = -1;

          break;
        }
      }
    }
  }

  long long checksum = 0, position = 0;
  for (auto const &i : layout) {
    // empty space
    if (i.first == -1) {
      position += i.second;
      continue;
    }

    auto [currentFileId, fileLength] = i;

    checksum += currentFileId *
                ((fileLength * position) + (fileLength * (fileLength - 1)) / 2);

    position += fileLength;
  }

  return checksum;
}
} // namespace part_two
