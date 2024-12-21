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

export module part_one;

#define DEBUG

export namespace part_one {

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

  int findShortestPath() {
    std::vector<std::vector<int>> distance(gridSize,
                                           std::vector<int>(gridSize, -1));
    std::queue<std::pair<int, int>> q;

    // Start from top-left corner
    q.push({0, 0});
    distance[0][0] = 0;

    while (!q.empty()) {
      auto const &[x, y] = q.front();
      q.pop();

      // If we reached the bottom-right corner
      if (x == gridSize - 1 && y == gridSize - 1) {
        return distance[x][y];
      }

      // Try all four directions
      for (int i = 0; i < 4; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (isValid(newX, newY) && distance[newX][newY] == -1) {
          distance[newX][newY] = distance[x][y] + 1;
          q.push({newX, newY});
        }
      }
    }

    return -1; // No path found
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

  auto totalBytes = 1024;
  for (size_t i = 0; i < (size_t)totalBytes && i < points.size(); i++) {
    finder.addCorruptedPoint(points[i].second, points[i].first);
  }

  std::cout << "Memory space after 1024 bytes:\n";
  finder.printGrid();

  int shortestPath = finder.findShortestPath();
  if (shortestPath != -1) {
    std::cout << "\nShortest path length: " << shortestPath << std::endl;
  } else {
    std::cout << "\nNo path exists to the exit!" << std::endl;
  }

  return shortestPath;
}
} // namespace part_one
