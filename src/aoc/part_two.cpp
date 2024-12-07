module;

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_two;

//#define DEBUG

export namespace part_two {

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
  }
};

auto is_stuck(const std::vector<std::string> &lines) -> bool {
  auto it = std::ranges::find_if(lines, [](const std::string &line) {
    return line.find('^') != std::string::npos;
  });

  long curr_i, curr_j = 0;
  if (it != lines.end()) {
    curr_i = std::distance(lines.begin(), it);
    curr_j = it->find('^');
  }

  auto um =
      std::unordered_map<std::pair<long, long>, std::vector<int>, pair_hash>();

  int direction = 0; // 0: up, 1: right, 2: down, 3: left
  std::vector<std::pair<long, long>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

  long ans = 0;
  while (true) {
    // if current position is within the map, add it to the set
    if (curr_i >= 0 && curr_i < lines.size() && curr_j >= 0 &&
        curr_j < lines[curr_i].size()) {
      auto &dirs = um[{curr_i, curr_j}];
      if (std::find(dirs.begin(), dirs.end(), direction) != dirs.end()) {
        return true;
      } else {
        dirs.push_back(direction);
      }
    }

    // find the next move based on direction
    auto next_i = curr_i + moves[direction].first;
    auto next_j = curr_j + moves[direction].second;

    // if next move is a hash, change direction
    if (next_i >= 0 && next_i < lines.size() && next_j >= 0 &&
        next_j < lines[next_i].size()) {
      if (lines[next_i][next_j] == '#') {
        direction = (direction + 1) % 4;
      } else {
        curr_i = next_i;
        curr_j = next_j;
      }
    } else {
      // outside the map already
      return false;
    }
  }
}

auto solve(const std::string &input) -> long {
  std::vector<std::string> lines;
  std::istringstream stream(input);
  for (std::string line; std::getline(stream, line);) {
    lines.push_back(line);
  }

  long ans = 0;
  for (auto &line : lines) {
    for (auto &c : line) {
      if (c == '.') {
        c = '#';
#ifdef DEBUG
        std::string result =
            std::accumulate(lines.begin() + 1, lines.end(),
                            lines[0], // Start with the first word
                            [](const std::string &a, const std::string &b) {
                              return a + "\n" + b; // Add newline between words
                            });
        std::cout << "Trying to check if " << std::endl
                  << result << std::endl
                  << "is stuck" << std::endl
                  << std::endl;
#endif
        if (is_stuck(lines)) {
          ans++;
        }
        c = '.';
      }
    }
  }

  return ans;
}

} // namespace part_two
