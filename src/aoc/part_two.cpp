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

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::vector<std::vector<char>> map;
  std::string path;

  std::string line;
  while (std::getline(stream, line)) {
    if (line.empty()) {
      break;
    }

    auto tmp = std::vector<char>(line.begin(), line.end());
    std::vector<char> row;
    for (const auto &ch : tmp) {
      if (ch == 'O') {
        row.push_back('[');
        row.push_back(']');
      } else if (ch == '@') {
        row.push_back('@');
        row.push_back('.');
      } else if (ch == '.') {
        row.push_back('.');
        row.push_back('.');
      } else if (ch == '#') {
        row.push_back('#');
        row.push_back('#');
      }
    }

    map.push_back(row);
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

#ifdef DEBUG
  for (const auto &row : map) {
    for (const auto &cell : row) {
      std::cout << cell;
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
#endif

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
      auto can_push = true;
      int i;

      // store the left [ of the boxes for simplicity
      std::vector<std::pair<int, int>> boxes;
      std::unordered_set<std::pair<int, int>, pair_hash> visited;
      std::queue<std::pair<int, int>> q;
      q.push({robot.first + delta.first, robot.second + delta.second});

      while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        auto current_char = map[curr.first][curr.second];
        if (current_char == '#') {
          can_push = false;
          break;
        }

        // normalize to [
        if (current_char == ']') {
          curr.second -= 1;
          current_char = '[';
        }

        if (visited.find(curr) != visited.end()) {
          continue;
        }

        visited.insert(curr);
        boxes.push_back(curr);

        // push delta due to [
        auto next_item_due_to_left =
            map[curr.first + delta.first][curr.second + delta.second];
        if (next_item_due_to_left != '.') {
          q.push({curr.first + delta.first, curr.second + delta.second});
        }

        // push delta due to ]
        std::pair<int, int> right_position = {curr.first, curr.second + 1};
        std::pair<int, int> next_due_to_right = {
            right_position.first + delta.first,
            right_position.second + delta.second};

        if (next_due_to_right == curr) {
          // the same box
          continue;
        }

        auto next_item_due_to_right = map[right_position.first + delta.first]
                                         [right_position.second + delta.second];
        if (next_item_due_to_right != '.') {
          q.push({right_position.first + delta.first,
                  right_position.second + delta.second});
        }
      }

      if (can_push) {
        // move every box in delta direction
        std::reverse(boxes.begin(), boxes.end());
        for (const auto &box : boxes) {
          map[box.first][box.second] = '.';     // left pos
          map[box.first][box.second + 1] = '.'; // right pos
          map[box.first + delta.first][box.second + delta.second] = '[';
          map[box.first + delta.first][box.second + 1 + delta.second] = ']';
        }

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
      if (map[i][j] == '[') {
        ans += 100 * i + j;
      }
    }
  }
  return ans;
}
} // namespace part_two
