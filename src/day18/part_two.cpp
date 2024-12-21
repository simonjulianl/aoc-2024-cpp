module;

#include <chrono>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <queue>
#include <ranges>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_two;

#define DEBUG

export namespace part_two {

auto parseInput(const std::string &input) -> std::vector<std::pair<int, int>> {
  std::vector<std::pair<int, int>> points;
  std::istringstream stream(input);
  std::string line;

  while (std::getline(stream, line)) {
    if (line.empty()) {
      continue;
    }

    size_t comma_pos = line.find(',');
    if (comma_pos == std::string::npos) {
      continue;
    }

    int x = std::stoi(line.substr(0, comma_pos));
    int y = std::stoi(line.substr(comma_pos + 1));
    points.push_back({x, y});
  }

  return points;
}

class MemoryPathFinder {
private:
  // Direction arrays for up, right, down, left movements
  const std::vector<int> dx = {-1, 0, 1, 0};
  const std::vector<int> dy = {0, 1, 0, -1};
  int gridSize;
  std::vector<std::vector<bool>> corrupted;

  bool isValid(int x, int y) {
    return x >= 0 && x < gridSize && y >= 0 && y < gridSize && !corrupted[x][y];
  }

public:
  MemoryPathFinder(int size) : gridSize(size) {
    corrupted =
        std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
  }

  void addCorruptedPoint(int x, int y) { corrupted[x][y] = true; }

  void reset() {
    corrupted = std::vector<std::vector<bool>>(
        gridSize, std::vector<bool>(gridSize, false));
  }

  bool pathExists() {
    std::vector<std::vector<bool>> visited(gridSize,
                                           std::vector<bool>(gridSize, false));
    std::queue<std::pair<int, int>> q;

    // Start from top-left corner
    q.push({0, 0});
    visited[0][0] = true;

    while (!q.empty()) {
      int x = q.front().first;
      int y = q.front().second;
      q.pop();

      // If we reached the bottom-right corner
      if (x == gridSize - 1 && y == gridSize - 1) {
        return true;
      }

      // Try all four directions
      for (int i = 0; i < 4; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (isValid(newX, newY) && !visited[newX][newY]) {
          visited[newX][newY] = true;
          q.push({newX, newY});
        }
      }
    }

    return false;
  }

  std::pair<int, int>
  findBlockingPoint(const std::vector<std::pair<int, int>> &points) {
    reset();

    for (size_t i = 0; i < points.size(); i++) {
      addCorruptedPoint(points[i].second, points[i].first);

      if (!pathExists()) {
        // Return the blocking point in (y,x) format
        return points[i];
      }
    }

    return {-1, -1}; // No blocking point found
  }

  void printGrid() {
    for (int i = 0; i < gridSize; i++) {
      for (int j = 0; j < gridSize; j++) {
        std::cout << (corrupted[i][j] ? '#' : '.');
      }
      std::cout << '\n';
    }
  }
};

auto solve(const std::string &input) -> long {
  auto points = parseInput(input);
  auto finder = MemoryPathFinder(71);

  auto blockingPoint = finder.findBlockingPoint(points);

  if (blockingPoint.first != -1) {
    // Print result in y,x format
    std::cout << blockingPoint.first << "," << blockingPoint.second
              << std::endl;
  } else {
    std::cout << "No blocking point found!" << std::endl;
  }

  return 0;
}
} // namespace part_two
