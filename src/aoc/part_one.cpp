module;

#include <chrono>
#include <iostream>
#include <iterator>
#include <limits>
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

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::vector<std::vector<char>> map;
  std::string path;

  std::string line;
  while (std::getline(stream, line)) {
    if (line.empty()) {
      break;
    }

    map.push_back(std::vector<char>(line.begin(), line.end()));
  }

  std::pair<int, int> robot;
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i][j] == '@') {
        robot = {i, j};
        break;
      }
    }
  }

  std::getline(stream, path);

  for (const auto &ch : path) {
    std::pair<int, int> delta;
    if (ch == '<') {
      delta = {0, -1};
    } else if (ch == '>') {
      delta = {0, 1};
    } else if (ch == '^') {
      delta = {-1, 0};
    } else if (ch == 'v') {
      delta = {1, 0};
    }

    auto next_char =
        map[robot.first + delta.first][robot.second + delta.second];
    if (next_char == '#') {
      continue; // Cannot do anything, it's a wall
    } else if (next_char == '.') {
      // move to the empty space
      map[robot.first][robot.second] = '.';
      robot.first += delta.first;
      robot.second += delta.second;
      map[robot.first][robot.second] = '@';
    } else {
      // It is a box, need to push it if possible
      // Find until the end
      bool can_push;
      int i;
      for (i = 1;; i++) {
        next_char =
            map[robot.first + i * delta.first][robot.second + i * delta.second];
        if (next_char == '#') {
          can_push = false;
          break;
        } else if (next_char == '.') {
          can_push = true;
          break;
        }
      }

      if (can_push) {
        map[robot.first + i * delta.first][robot.second + i * delta.second] =
            'O';
        map[robot.first][robot.second] = '.';
        robot.first += delta.first;
        robot.second += delta.second;
        map[robot.first][robot.second] = '@';
      }
    }

    // Output the map
  }

  for (const auto &row : map) {
    for (const auto &cell : row) {
      std::cout << cell;
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
  // Count the location of the boxes
  long ans = 0;
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i][j] == 'O') {
        ans += 100 * i + j;
      }
    }
  }
  return ans;
}
} // namespace part_one
