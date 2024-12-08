module;

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_two;

// #define DEBUG

export namespace part_two {

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
  }
};

auto solve(const std::string &input) -> long {
  auto freq_map = std::unordered_map<char, std::vector<std::pair<int, int>>>();
  auto visited_nodes = std::unordered_set<std::pair<int, int>, pair_hash>();

  int i = 0;
  long max_j = 0;

  for (const auto &line : input | std::views::split('\n')) {
    max_j = line.size();
    for (size_t j = 0; j < line.size(); j++) {
      auto ch = line[j];
      if (ch != '.') {
        freq_map[ch].push_back({i, j});
      }
    }

    i++;
  }

  long max_i = i;

  long ans = 0;
  for (const auto &[ch, locations] : freq_map) {
    for (const auto &first : locations) {
      for (const auto &second : locations) {
        if (first == second)
          continue;

        auto di = second.first - first.first;
        auto dj = second.second - first.second;

        long multiplier = 0;
        while (true) {
          auto new_i = second.first + multiplier * di;
          auto new_j = second.second + multiplier * dj;

          if (new_i >= 0 && new_i < max_i && new_j >= 0 && new_j < max_j) {
            if (visited_nodes.find({new_i, new_j}) == visited_nodes.end()) {
              ans++;
              visited_nodes.insert({new_i, new_j});

#ifdef DEBUG
              std::cout << "Found " << ch << " at " << new_i << ", " << new_j
                        << std::endl;
#endif
            }
            multiplier++;
          } else {
            break;
          }
        }
      }
    }
  }

  return ans;
}
} // namespace part_two
