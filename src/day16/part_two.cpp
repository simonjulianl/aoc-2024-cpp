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

export module part_two;

// #define DEBUG

export namespace part_two {

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
  }
};

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

auto getTilesInBestPath(
    std::unordered_set<std::pair<int, int>, pair_hash> &visited_points,
    long bestPathScore,
    std::unordered_set<std::tuple<int, int, int>, tuple_hash> &cached_path,
    std::vector<std::tuple<int, int, int, long>> &path,
    const std::vector<std::string> &maze) -> void {
  auto [i, j, dir, score] = path.back();
  if (score > bestPathScore) {
    return;
  }

  if (maze[i][j] == 'E') {
    // Current path score is at the end of the path
    if (score == bestPathScore) {
      for (const auto &[vi, vj, dir_not_used, score_not_used] : path) {
        visited_points.insert({vi, vj});
      }
    }
    return;
  }

  // Based on the current point, there are 3 paths to explore
  // 1. Go straight
  auto [di, dj] = getDelta(dir);

  auto new_i = i + di;
  auto new_j = j + dj;

  if (new_i >= 0 && new_i < (int)maze.size() && new_j >= 0 &&
      new_j < (int)maze[new_i].size() && maze[new_i][new_j] != '#') {
    auto new_state = std::make_tuple(new_i, new_j, dir);

    if (cached_path.find(new_state) == cached_path.end()) {
      // This path has never been visited before
      cached_path.insert(new_state);
      path.push_back({new_i, new_j, dir, score + 1});

      getTilesInBestPath(visited_points, bestPathScore, cached_path, path,
                         maze);

      // Backtrack
      path.pop_back();
      cached_path.erase(new_state);
    }
  }

  auto consideredDirs = std::vector<int>{(dir + 1) % 4, (dir + 3) % 4};
  // rotate left or right
  for (const auto &new_dir : consideredDirs) {
    auto new_state = std::make_tuple(i, j, new_dir);
    if (cached_path.find(new_state) == cached_path.end()) {
      // This path has never been visited before
      cached_path.insert(new_state);
      path.push_back({i, j, new_dir, score + 1000});

      getTilesInBestPath(visited_points, bestPathScore, cached_path, path,
                         maze);

      // Backtrack
      path.pop_back();
      cached_path.erase(new_state);
    }
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

  long best_path_score = std::numeric_limits<long>::max();
  std::pair<int, int> end;
  while (!q.empty()) {
    auto [i, j, dir] = q.front();
    q.pop();

    if (maze[i][j] == 'E') {
      end = {i, j};
      best_path_score = std::min(best_path_score, visited_nodes[{i, j, dir}]);
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

  // Given best_path_score, try to traverse all paths using dfs that lead to
  // this best path score

  // Brute force approach, try all paths, do not work
  //  std::unordered_set<std::tuple<int, int, int>, tuple_hash> cached_path;
  //  std::vector<std::tuple<int, int, int, long>> path;
  //  path.push_back({start.first, start.second, 1, 0});
  //  getTilesInBestPath(visited_points, best_path_score, cached_path, path,
  //  maze);

  std::unordered_set<std::pair<int, int>, pair_hash> visited_points;
  std::queue<std::tuple<int, int, int, long>> q2;

  // Back track from the end, try to find possible edges using visited nodes
  for (int i = 0; i < 4; i++) {
    q2.push({end.first, end.second, i, best_path_score});
  }

  while (!q2.empty()) {
    auto [i, j, dir, score] = q2.front();
    q2.pop();

    visited_points.insert({i, j});

    auto [di, dj] = getDelta(dir);

    // Reverse walking back
    int new_i = i - di;
    int new_j = j - dj;
    if (new_i >= 0 && new_i < (int)maze.size() && new_j >= 0 &&
        new_j < (int)maze[new_i].size() && maze[new_i][new_j] != '#') {
      if (visited_nodes.find({new_i, new_j, dir}) != visited_nodes.end()) {
        auto cost = visited_nodes[{new_i, new_j, dir}];
        if (cost == score - 1) {
          // That path must be part of the best path
          auto new_state = std::make_tuple(new_i, new_j, dir, score - 1);
          q2.push(new_state);
        }
      }
    }

    auto consideredDirs = std::vector<int>{(dir + 1) % 4, (dir + 3) % 4};
    for (const auto &new_dir : consideredDirs) {
      if (visited_nodes.find({i,  j, new_dir}) != visited_nodes.end()) {
        auto cost = visited_nodes[{i, j, new_dir}];
        if (cost == score - 1000) {
          // That path must be part of the best path
          auto new_state = std::make_tuple(i, j, new_dir, score - 1000);
          q2.push(new_state);
        }
      }
    }
  }

  return (long)visited_points.size();
}
} // namespace part_two
