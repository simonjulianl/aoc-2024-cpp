module;

#include <chrono>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <ranges>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_one;

// #define DEBUG

export namespace part_one {

struct tuple_hash {
  template <class T>
  std::size_t operator()(const std::tuple<T, T, T> &tuple) const {
    auto first = std::get<0>(tuple);
    auto second = std::get<1>(tuple);
    auto third = std::get<2>(tuple);
    return std::hash<T>{}(first) ^ std::hash<T>{}(second) ^
           std::hash<T>{}(third);
  }
};

auto getDelta(int dir) -> std::pair<int, int> {
  switch (dir) {
  case 0:
    return {-1, 0};
  case 1:
    return {0, 1};
  case 2:
    return {1, 0};
  default:
    return {0, -1};
  }
}

auto solve(const std::string &input) -> long {
  std::vector<std::string> maze;
  std::ranges::copy(input | std::views::split('\n') |
                        std::views::transform([](auto &&line) {
                          return std::string(line.begin(), line.end());
                        }),
                    std::back_inserter(maze));

  std::pair<int, int> start;
  for (int i = 0; i < (int)maze.size(); i++) {
    for (int j = 0; j < (int)maze[i].size(); j++) {
      if (maze[i][j] == 'S') {
        start = {i, j};
        break;
      }
    }
  }

  std::queue<std::tuple<int, int, int>> q;
  // define N E S W = 0 1 2 3
  std::unordered_map<std::tuple<int, int, int>, long, tuple_hash> visited_nodes;

  auto init = std::make_tuple(start.first, start.second, 1);
  q.push(init);
  visited_nodes[init] = 0;

  long ans = std::numeric_limits<long>::max();
  while (!q.empty()) {
    auto [i, j, dir] = q.front();
    q.pop();

    if (maze[i][j] == 'E') {
      ans = std::min(ans, visited_nodes[{i, j, dir}]);
    }

    // Go straight
    auto [di, dj] = getDelta(dir);

    int new_i = i + di;
    int new_j = j + dj;
    // check if new_i and new_j are within bounds and do not hit wall #
    if (new_i >= 0 && new_i < (int)maze.size() && new_j >= 0 &&
        new_j < (int)maze[new_i].size() && maze[new_i][new_j] != '#') {
      auto new_state = std::make_tuple(new_i, new_j, dir);
      if (visited_nodes.find(new_state) == visited_nodes.end()) {
        visited_nodes[new_state] = visited_nodes[{i, j, dir}] + 1;
        q.push(new_state);
      } else {
        auto old_score = visited_nodes[new_state];
        auto new_score = visited_nodes[{i, j, dir}] + 1;

        if (new_score < old_score) {
          visited_nodes[new_state] = new_score;
          q.push(new_state);
        }
      }
    }

    auto consideredDirs = std::vector<int>{(dir + 1) % 4, (dir + 3) % 4};
    // rotate left or right
    for (const auto &new_dir : consideredDirs) {
      auto new_state = std::make_tuple(i, j, new_dir);
      if (visited_nodes.find(new_state) == visited_nodes.end()) {
        visited_nodes[new_state] = visited_nodes[{i, j, dir}] + 1000;
        q.push(new_state);
      } else {
        auto old_score = visited_nodes[new_state];
        auto new_score = visited_nodes[{i, j, dir}] + 1000;

        if (new_score < old_score) {
          visited_nodes[new_state] = new_score;
          q.push(new_state);
        }
      }
    }
  }

  return ans;
}
} // namespace part_one
