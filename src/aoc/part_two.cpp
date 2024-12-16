module;

#include <chrono>
#include <iostream>
#include <limits>
#include <queue>
#include <ranges>
#include <regex>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

export module part_two;

// #define DEBUG

export namespace part_two {

// WTF... this doesn't work...
auto contain3x3Block(const std::vector<std::vector<char>> &map) -> bool {
  long tall = map.size();
  long wide = map[0].size();

  for (int i = 0; i < tall - 3; i++) {
    for (int j = 0; j < wide - 3; j++) {
      auto isBlock = true;
      for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
          if (map[i + k][j + l] != 'R') {
            isBlock = false;
          }
        }
      }
      if (isBlock) {
        return true;
      }
    }
  }

  return false;
}

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
  }
};

auto countLargestConnectedComponent(const std::vector<std::vector<char>> &map)
    -> long {
  long ans = 0;
  std::unordered_set<std::pair<long, long>, pair_hash> visited;

  long tall = map.size();
  long wide = map[0].size();
  for (int i = 0; i < tall; i++) {
    for (int j = 0; j < wide; j++) {
      if (map[i][j] != 'R') {
        continue;
      }

      if (visited.find({i, j}) != visited.end()) {
        continue;
      }

      visited.insert({i, j});
      std::queue<std::pair<long, long>> q;
      q.push({i, j});
      long size = 0;
      while (!q.empty()) {
        auto &[curr_i, curr_j] = q.front();
        q.pop();
        size++;

        // 8 direction neighbour
        for (const auto &[di, dj] :
             std::vector<std::pair<long, long>>{{-1, 0},
                                                {1, 0},
                                                {0, -1},
                                                {0, 1},
                                                {-1, -1},
                                                {1, 1},
                                                {-1, 1},
                                                {1, -1}}) {
          long next_i = curr_i + di;
          long next_j = curr_j + dj;

          if (next_i >= 0 && next_i < tall && next_j >= 0 && next_j < wide) {
            if (map[next_i][next_j] == 'R') {
              if (visited.find({next_i, next_j}) == visited.end()) {
                q.push({next_i, next_j});
                visited.insert({next_i, next_j});
              }
            }
          }
        }
      }

      ans = std::max(ans, size);
    }
  }
  return ans;
}

auto solve(const std::string &input) -> long {
  std::regex pattern(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");
  std::smatch match;
  std::vector<std::pair<std::pair<long, long>, std::pair<long, long>>> robots;

  for (const auto &line : input | std::views::split('\n')) {
    auto str = std::string(line.begin(), line.end());
    if (std::regex_match(str, match, pattern)) {
      long px = std::stol(match[1]);
      long py = std::stol(match[2]);
      long vx = std::stol(match[3]);
      long vy = std::stol(match[4]);
      robots.push_back({{px, py}, {vx, vy}});
    }
  }

  long wide = 101, tall = 103;

  long time = 0;
  while (true) {
    for (auto &robot : robots) {
      robot.first.first += robot.second.first;
      robot.first.second += robot.second.second;

      robot.first.first %= wide;
      robot.first.second %= tall;

      if (robot.first.first < 0) {
        robot.first.first += wide;
      }

      if (robot.first.second < 0) {
        robot.first.second += tall;
      }
    }

    // Hopefully the xmas tree contains a blob of 9 robots...
    std::vector<std::vector<char>> map(tall, std::vector<char>(wide, '.'));

    // Mark the robots on the map
    for (const auto &robot : robots) {
      long x = robot.first.first;
      long y = robot.first.second;

      map[y][x] = 'R'; // 'R' to represent a robot
    }

    auto largestConnectedComponent = countLargestConnectedComponent(map);
    if (largestConnectedComponent >= 20) {
      std::cout << "Time: " << time << std::endl;
      for (const auto &row : map) {
        for (const auto &cell : row) {
          std::cout << cell;
        }
        std::cout << '\n';
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    time++;
  }

  return 1;
}
} // namespace part_two
