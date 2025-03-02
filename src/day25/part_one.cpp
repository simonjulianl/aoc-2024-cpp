module;

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_one;

#define DEBUG

export namespace part_one {

auto splitStringBy(const std::string &input, const std::string &separator)
    -> std::vector<std::string> {
  std::string inputCopy = input;
  std::vector<std::string> parts;
  size_t pos = 0;
  while ((pos = inputCopy.find(separator)) != std::string::npos) {
    parts.push_back(inputCopy.substr(0, pos));
    inputCopy.erase(0, pos + separator.length());
  }

  parts.push_back(inputCopy);

  return parts;
}

auto getLockKeyRepresentation(const std::string &keyLock)
    -> std::tuple<bool, std::vector<int>> {
  auto keyLockParts = splitStringBy(keyLock, "\n");
  int rows = keyLockParts.size();
  int cols = keyLockParts[0].size();
  std::vector<int> heights(cols, 0);

  bool isLock = false;
  for (auto col = 0; col < cols; col++) {
    if (keyLockParts[0][col] == '#') { // Check if it's a lock
      isLock = true;
      auto count = 0;
      for (auto row = 1; row < rows; row++) {
        if (keyLockParts[row][col] == '#') {
          count++;
        }
      }
      heights[col] = count;
    } else { // It's a key
      auto count = 0;
      for (auto row = 1; row < rows; row++) {
        if (keyLockParts[row][col] == '.') {
          count++;
        }
      }
      heights[col] = rows - 2 - count; // Subtract 2 because of the borders
    }
  }

  return std::make_tuple(isLock, heights);
}

auto doesKeyFitLock(const std::vector<int> &key, const std::vector<int> &lock,
                    const int &lockSize) -> bool {
  for (auto i = 0; i < key.size(); i++) {
    if (key[i] + lock[i] > lockSize) {
      return false;
    }
  }

  return true;
}

auto solve(const std::string &input) -> long {
  auto keyLocks = splitStringBy(input, "\n\n");

  std::vector<std::vector<int>> locks;
  std::vector<std::vector<int>> keys;

  for (const auto &keyLock : keyLocks) {
    auto [isLock, heights] = getLockKeyRepresentation(keyLock);
    if (isLock) {
      locks.push_back(heights);
    } else {
      keys.push_back(heights);
    }
  }

  auto matchingPair = 0;
  for (auto key : keys) {
    for (auto lock : locks) {
      if (doesKeyFitLock(key, lock, 5)) { // 5 is just from manual observation
        matchingPair++;
      }
    }
  }

  return matchingPair;
}
} // namespace part_one
