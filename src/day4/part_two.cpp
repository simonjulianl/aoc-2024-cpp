module;

#include <iostream>
#include <regex>
#include <vector>

export module part_two;

export namespace part_two {

auto isMas(const std::vector<std::string> &map, const int &i, const int &j,
           const int &dir) -> bool {

  long first_i, first_j, second_i, second_j;

  if (dir == 0) {
    first_i = i + 1;
    first_j = j + 1;
    second_i = i - 1;
    second_j = j - 1;
  } else {
    first_i = i - 1;
    first_j = j + 1;
    second_i = i + 1;
    second_j = j - 1;
  }

  if (first_i < 0 || first_i >= static_cast<int>(map.size()) || first_j < 0 ||
      first_j >= static_cast<int>(map[0].size())) {
    return false;
  }

  if (second_i < 0 || second_i >= static_cast<int>(map.size()) ||
      second_j < 0 || second_j >= static_cast<int>(map[0].size())) {
    return false;
  }

  auto first_char = map[first_i][first_j];
  auto second_char = map[second_i][second_j];

  return (first_char == 'M' && second_char == 'S') ||
         (first_char == 'S' && second_char == 'M');
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
      if (c == 'A') {
        if (isMas(lines, i, j, 0) && isMas(lines, i, j, 1)) {
          ans++;
        }
      }
    }
  }

  return ans;
}
} // namespace part_two