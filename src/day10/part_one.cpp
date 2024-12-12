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

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
  }
};

auto count_score(const std::vector<std::string> &map, int i, int j) -> long {
  long score = 0;
  std::unordered_set<std::pair<int, int>, pair_hash> visited;
  std::queue<std::pair<int, int>> q;

  int width = map[0].size();
  int height = map.size();

  q.push({i, j});
  while (!q.empty()) {
    auto [curr_i, curr_j] = q.front();
    q.pop();

    if (visited.find({curr_i, curr_j}) != visited.end()) {
      continue;
    }

    visited.insert({curr_i, curr_j});

    auto current_char = map[curr_i][curr_j];
    if (current_char == '9') {
      score++;
    }

    auto next_char = (char)(current_char + 1);

    for (const auto &[di, dj] :
         std::vector<std::pair<int, int>>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
      int next_i = curr_i + di;
      int next_j = curr_j + dj;

      if (next_i >= 0 && next_i < height && next_j >= 0 && next_j < width) {
        if (map[next_i][next_j] == next_char) {
          if (visited.find({next_i, next_j}) == visited.end()) {
            q.push({next_i, next_j});
          }
        }
      }
    }
  }
  return score;
}

auto solve(const std::string &input) -> long {
  std::vector<std::string> map;

  for (const auto &line : input | std::views::split('\n')) {
    map.push_back(std::string(std::begin(line), std::end(line)));
  }

  long score = 0;
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      const char c = map[i][j];
      if (c == '0') {
        // count the score of this trailhead
        auto s = count_score(map, i, j);
        score += s;
      }
    }
  }

  return score;
}
} // namespace part_one
