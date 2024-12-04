module;

#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

export module part_one;

// #define DEBUG

export namespace part_one {

static const std::string xmas = "XMAS";

auto isXmas(const std::vector<std::string> &map, int idx, const int &i,
            const int &j, const int &di, const int &dj) -> bool {

  if (i < 0 || i >= static_cast<int>(map.size()) || j < 0 ||
      j >= static_cast<int>(map[0].size())) {
    return false;
  }

  auto expected_char = xmas[idx];
  auto current_char = map[i][j];

  if (expected_char != current_char) {
    return false;
  }

  if (idx == static_cast<int>(xmas.size() - 1)) {
    return true;
  }

  auto new_i = i + di;
  auto new_j = j + dj;

  return isXmas(map, idx + 1, new_i, new_j, di, dj);
}

auto solve(const std::string &input) -> long {
  auto stream = std::istringstream{input};
  std::vector<std::string> lines;

  for (auto line = std::string(); std::getline(stream, line);) {
    lines.push_back(line);
  }

  std::vector<int> di = {0, 1, -1};
  std::vector<int> dj = {0, 1, -1};

  long ans = 0;
  for (size_t i = 0; i < lines.size(); i++) {
    auto line = lines[i];
    for (size_t j = 0; j < line.size(); j++) {
      auto c = line[j];
      if (c == 'X') {
        for (auto &dii : di) {
          for (auto &djj : dj) {
            if (dii == 0 && djj == 0) {
              continue;
            }

            if (isXmas(lines, 0, i, j, dii, djj)) {
              ans++;
            }
          }
        }
      }
    }
  }

  return ans;
}
} // namespace part_one
