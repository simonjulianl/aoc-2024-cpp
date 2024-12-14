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

export module part_two;

#define DEBUG

export namespace part_two {

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

  // Think of each box having 4 corners, so total sides = total true corners
  long true_corners = 0;
  long area = 0;
  auto init_char = map[i][j];

  while (!q.empty()) {
    auto [curr_i, curr_j] = q.front();
    q.pop();

    // For each valid character, increase the area and offset
    area++;

    long valid_neighbours = 0;
    std ::vector<std::pair<int, int>> valid_neighbours_pos{
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    auto UP = false, DOWN = false, LEFT = false, RIGHT = false;
    auto UP_RIGHT = false, UP_LEFT = false, DOWN_RIGHT = false,
         DOWN_LEFT = false;
    for (auto const &[di, dj] : std::vector<std::pair<int, int>>{{-1, 0},
                                                                 {1, 0},
                                                                 {0, -1},
                                                                 {0, 1},
                                                                 {-1, 1},
                                                                 {-1, -1},
                                                                 {1, 1},
                                                                 {1, -1}}) {
      auto new_i = curr_i + di;
      auto new_j = curr_j + dj;

      if (new_i < 0 || new_i >= map.size() || new_j < 0 ||
          new_j >= map[i].size()) {
        continue;
      }

      auto new_char = map[new_i][new_j];
      if (new_char == init_char) {
        if (std::find(std::begin(valid_neighbours_pos),
                      std::end(valid_neighbours_pos), std::make_pair(di, dj)) !=
            std::end(valid_neighbours_pos)) {
          valid_neighbours++;
          if (visited.find({new_i, new_j}) == visited.end()) {
            q.push({new_i, new_j});
            visited.insert({new_i, new_j});
          }
        }

        if (std::make_pair(di, dj) == std::make_pair(-1, 0)) {
          UP = true;
        } else if (std::make_pair(di, dj) == std::make_pair(1, 0)) {
          DOWN = true;
        } else if (std::make_pair(di, dj) == std::make_pair(0, -1)) {
          LEFT = true;
        } else if (std::make_pair(di, dj) == std::make_pair(0, 1)) {
          RIGHT = true;
        } else if (std::make_pair(di, dj) == std::make_pair(-1, 1)) {
          UP_RIGHT = true;
        } else if (std::make_pair(di, dj) == std::make_pair(-1, -1)) {
          UP_LEFT = true;
        } else if (std::make_pair(di, dj) == std::make_pair(1, 1)) {
          DOWN_RIGHT = true;
        } else if (std::make_pair(di, dj) == std::make_pair(1, -1)) {
          DOWN_LEFT = true;
        }
      }
    }

    auto countOuterCorners = [&]() -> long {
      auto outerCorners = 0;
      if (UP && RIGHT && !UP_RIGHT) {
        outerCorners++;
      }

      if (UP && LEFT && !UP_LEFT) {
        outerCorners++;
      }

      if (DOWN && RIGHT && !DOWN_RIGHT) {
        outerCorners++;
      }

      if (DOWN && LEFT && !DOWN_LEFT) {
        outerCorners++;
      }
      return outerCorners;
    };

    auto temp_corners = 0;
    if (valid_neighbours == 0) {
      temp_corners = 4; // inner corners
    } else if (valid_neighbours == 1) {
      temp_corners = 2; // inner corners
    } else if (valid_neighbours == 2) {
      if ((UP && DOWN) || (LEFT && RIGHT)) {
        // count ins
        temp_corners = countOuterCorners();
      } else {
        // if it's an L shape, one of the corner must be inner corner
        temp_corners = 1 + countOuterCorners();
      }
    } else if (valid_neighbours == 3 || valid_neighbours == 4) {
      // Every possible "corner" must be outer corners
      temp_corners = countOuterCorners();
    }

    true_corners += temp_corners;
  }

  return area * true_corners;
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
} // namespace part_two
