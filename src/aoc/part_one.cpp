module;

#include <algorithm>
#include <cmath>
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

//#define DEBUG

export namespace part_one {

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
  }
};

using PairSet = std::unordered_set<std::pair<int, int>, pair_hash>;

auto getPrice(const std::vector<std::string> &map, PairSet &visited, int i,
              int j) -> long {
  std::queue<std::pair<int, int>> q;
  visited.insert({i, j});
  q.push({i, j});

  long offset = 0;
  long area = 0;
  auto init_char = map[i][j];

  while (!q.empty()) {
    auto [curr_i, curr_j] = q.front();
    q.pop();

    // For each valid character, increase the area and offset
    area++;

    for (auto const &[di, dj] :
         std::vector<std::pair<int, int>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
      auto new_i = curr_i + di;
      auto new_j = curr_j + dj;
      if (new_i < 0 || new_i >= map.size() || new_j < 0 ||
          new_j >= map[i].size()) {
        continue;
      }

      auto new_char = map[new_i][new_j];
      if (new_char == init_char) {
        offset++; // neighbouring same char
        if (visited.find({new_i, new_j}) == visited.end()) {
          q.push({new_i, new_j});
          visited.insert({new_i, new_j});
        }
      }
    }
  }

  auto perimeter = 4 * area - offset;

#ifdef DEBUG
  std::cout << "Char: " << init_char << "| Area: " << area
            << " Perimeter: " << perimeter << std::endl;
#endif
  return area * perimeter;
}

auto solve(const std::string &input) -> long {
  std::vector<std::string> map;
  PairSet visited;

  for (const auto &line : input | std::views::split('\n')) {
    map.push_back(std::string(std::begin(line), std::end(line)));
  }

  long ans = 0;
  for (size_t i = 0; i < map.size(); i++) {
    for (size_t j = 0; j < map[i].size(); j++) {
      if (visited.find({i, j}) == visited.end()) {
        // Try to bfs the current region
        ans += getPrice(map, visited, i, j);
      }
    }
  }
  return ans;
}
} // namespace part_one
