module;

#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

export module part_two;

// #define DEBUG
using Position = std::pair<int, int>;

struct PositionHash {
  std::size_t operator()(const Position &p) const {
    return std::hash<int>{}(p.first) ^ std::hash<int>{}(p.second);
  }
};

class RaceSolver {
private:
  std::vector<std::vector<char>> grid;
  Position start, end;
  int rows, col;

  auto findStartEnd() -> void {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < col; j++) {
        if (grid[i][j] == 'S') {
          start = {i, j};
          grid[i][j] = '.';
        } else if (grid[i][j] == 'E') {
          end = {i, j};
          grid[i][j] = '.';
        }
      }
    }
  }

  auto getNeighbours(const Position &pos) -> std::vector<Position> const {
    std::vector<Position> neighbours;
    const std::vector<std::pair<int, int>> dirs = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (const auto &dir : dirs) {
      int newR = pos.first + dir.first;
      int newC = pos.second + dir.second;

      if (newR >= 0 && newR < rows && newC >= 0 && newC < col) {
        if (grid[newR][newC] == '.') {
          neighbours.push_back({newR, newC});
        }
      }
    }

    return neighbours;
  }

  auto shortestPath(const Position &start) {
    std::unordered_map<Position, int, PositionHash> distances;
    std::queue<Position> q;

    distances[start] = 0;
    q.push(start);

    while (!q.empty()) {
      Position current = q.front();
      q.pop();

      for (const auto &neighbour : getNeighbours(current)) {
        if (distances.find(neighbour) == distances.end()) {
          distances[neighbour] = distances[current] + 1;
          q.push(neighbour);
        }
      }
    }

    return distances;
  }

  int manhattanDist(const Position &p1, const Position &p2) {
    return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
  }

public:
  RaceSolver(const std::string &input) {
    std::istringstream iss(input);
    std::string line;
    while (std::getline(iss, line)) {
      if (!line.empty()) {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
      }
    }

    rows = grid.size();
    col = grid[0].size();
    findStartEnd();
  }

  auto solve() {
    auto distFromStart = shortestPath(start);
    auto distFromEnd = shortestPath(end);

    // Assume path from start to end exists
    int normalPathLength = distFromStart[end];
    std::vector<int> savings;

    for (int r1 = 0; r1 < rows; r1++) {
      for (int c1 = 0; c1 < col; c1++) {
        if (grid[r1][c1] != '.')
          continue;

        Position p1 = {r1, c1};
        // not even reachable from start
        if (distFromStart.find(p1) == distFromStart.end())
          continue;

        // Try all possible cheat end positions within 20 moves
        const int moveLimit = 20;
        for (int r2 = std::max(0, r1 - moveLimit); r2 < std::min(rows, r1 + 1 + moveLimit); r2++) {
          for (int c2 = std::max(0, c1 - moveLimit); c2 < std::min(col, c1 + 1 + moveLimit); c2++) {
            Position p2 = {r2, c2};
            if (grid[r2][c2] != '.' || manhattanDist(p1, p2) > moveLimit ||
                distFromEnd.find(p2) == distFromEnd.end())
              continue;

            int cheatLength =
                distFromStart[p1] + manhattanDist(p1, p2) + distFromEnd[p2];
            if (cheatLength < normalPathLength) {
              savings.push_back(normalPathLength - cheatLength);
            }
          }
        }
      }
    }

    const int limit = 100;
    return std::count_if(savings.begin(), savings.end(),
                         [](int s) { return s >= limit; });
  }
};

export namespace part_two {

auto solve(const std::string &input) -> long {
  auto solver = RaceSolver(input);
  auto ans = solver.solve();
  return ans;
}
} // namespace part_two
