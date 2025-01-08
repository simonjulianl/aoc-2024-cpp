module;

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

const std::vector<std::string> NUMERIC = {"789", "456", "123", " 0A"};
const std::vector<std::string> DIRECTIONAL = {" ^A", "<v>"};

// Custom hash function for vectors
struct VectorHash {
  template <typename T> std::size_t operator()(const std::vector<T> &v) const {
    std::size_t hash = v.size();
    for (const T &item : v) {
      hash ^= std::hash<T>{}(item) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
  }
};

// Cache key structure
struct CacheKey {
  std::vector<std::string> keypad;
  std::string start;
  std::string end;
  int links;

  bool operator==(const CacheKey &other) const {
    return keypad == other.keypad && start == other.start && end == other.end &&
           links == other.links;
  }
};

// Custom hash function for CacheKey
struct CacheKeyHash {
  std::size_t operator()(const CacheKey &k) const {
    return VectorHash()(k.keypad) ^ std::hash<std::string>{}(k.start) ^
           std::hash<std::string>{}(k.end) ^ std::hash<int>{}(k.links);
  }
};

// Find position of a character in keypad
std::pair<int, int> find_pos(const std::vector<std::string> &keypad, char c) {
  for (int y = 0; y < keypad.size(); y++) {
    for (int x = 0; x < keypad[y].length(); x++) {
      if (keypad[y][x] == c) {
        return {x, y};
      }
    }
  }
  return {-1, -1};
}

// Check if a path is valid
bool is_valid_path(const std::vector<std::string> &keypad, int x, int y,
                   const std::string &path) {
  for (char dir : path) {
    switch (dir) {
    case '<':
      x--;
      break;
    case '>':
      x++;
      break;
    case '^':
      y--;
      break;
    case 'v':
      y++;
      break;
    }
    if (y < 0 || y >= keypad.size() || x < 0 || x >= keypad[y].length() ||
        keypad[y][x] == ' ') {
      return false;
    }
  }
  return true;
}

// Get all valid paths between two positions
std::set<std::string> paths_between(const std::vector<std::string> &keypad,
                                    char start, char end) {
  auto [x1, y1] = find_pos(keypad, start);
  auto [x2, y2] = find_pos(keypad, end);

  std::set<std::string> valid_paths;
  std::string hor = std::string(abs(x2 - x1), x2 > x1 ? '>' : '<');
  std::string ver = std::string(abs(y2 - y1), y2 > y1 ? 'v' : '^');

  // Try both horizontal+vertical and vertical+horizontal combinations
  std::vector<std::string> attempts = {hor + ver, ver + hor};
  for (const auto &path : attempts) {
    if (is_valid_path(keypad, x1, y1, path)) {
      valid_paths.insert(path + 'A');
    }
  }
  return valid_paths;
}

std::unordered_map<CacheKey, int, CacheKeyHash> cost_cache;

// Calculate cost between two positions
int cost_between(const std::vector<std::string> &keypad, char start, char end,
                 int links) {
  if (links == 0)
    return 1;

  CacheKey key{keypad, std::string(1, start), std::string(1, end), links};
  if (cost_cache.count(key))
    return cost_cache[key];

  int min_cost = INT_MAX;
  for (const auto &path : paths_between(keypad, start, end)) {
    min_cost = std::min(min_cost, cost(DIRECTIONAL, path, links - 1));
  }
  cost_cache[key] = min_cost;
  return min_cost;
}

// Calculate total cost for a sequence
int cost(const std::vector<std::string> &keypad, const std::string &keys,
         int links) {
  std::string sequence = "A" + keys;
  int total_cost = 0;
  for (size_t i = 0; i < sequence.length() - 1; i++) {
    total_cost += cost_between(keypad, sequence[i], sequence[i + 1], links);
  }
  return total_cost;
}

// Calculate complexity of a code
int complexity(const std::string &code, int robots) {
  std::string numeric_part = code.substr(0, code.length() - 1);
  while (numeric_part[0] == '0' && numeric_part.length() > 1) {
    numeric_part = numeric_part.substr(1);
  }
  return cost(NUMERIC, code, robots + 1) * std::stoi(numeric_part);
}

export module part_one;

#define DEBUG

export namespace part_one {

const std::vector<std::string> NUMERIC = {"789", "456", "123", " 0A"};
const std::vector<std::string> DIRECTIONAL = {" ^A", "<v>"};

// Custom hash function for vectors
struct VectorHash {
  template <typename T> std::size_t operator()(const std::vector<T> &v) const {
    std::size_t hash = v.size();
    for (const T &item : v) {
      hash ^= std::hash<T>{}(item) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
  }
};

// Cache key structure
struct CacheKey {
  std::vector<std::string> keypad;
  std::string start;
  std::string end;
  int links;

  bool operator==(const CacheKey &other) const {
    return keypad == other.keypad && start == other.start && end == other.end &&
           links == other.links;
  }
};

// Custom hash function for CacheKey
struct CacheKeyHash {
  std::size_t operator()(const CacheKey &k) const {
    return VectorHash()(k.keypad) ^ std::hash<std::string>{}(k.start) ^
           std::hash<std::string>{}(k.end) ^ std::hash<int>{}(k.links);
  }
};

// Find position of a character in keypad
std::pair<int, int> find_pos(const std::vector<std::string> &keypad, char c) {
  for (int y = 0; y < keypad.size(); y++) {
    for (int x = 0; x < keypad[y].length(); x++) {
      if (keypad[y][x] == c) {
        return {x, y};
      }
    }
  }
  return {-1, -1};
}

// Check if a path is valid
bool is_valid_path(const std::vector<std::string> &keypad, int x, int y,
                   const std::string &path) {
  for (char dir : path) {
    switch (dir) {
    case '<':
      x--;
      break;
    case '>':
      x++;
      break;
    case '^':
      y--;
      break;
    case 'v':
      y++;
      break;
    }
    if (y < 0 || y >= keypad.size() || x < 0 || x >= keypad[y].length() ||
        keypad[y][x] == ' ') {
      return false;
    }
  }
  return true;
}

// Get all valid paths between two positions
std::set<std::string> paths_between(const std::vector<std::string> &keypad,
                                    char start, char end) {
  auto [x1, y1] = find_pos(keypad, start);
  auto [x2, y2] = find_pos(keypad, end);

  std::set<std::string> valid_paths;
  std::string hor = std::string(abs(x2 - x1), x2 > x1 ? '>' : '<');
  std::string ver = std::string(abs(y2 - y1), y2 > y1 ? 'v' : '^');

  // Try both horizontal+vertical and vertical+horizontal combinations
  std::vector<std::string> attempts = {hor + ver, ver + hor};
  for (const auto &path : attempts) {
    if (is_valid_path(keypad, x1, y1, path)) {
      valid_paths.insert(path + 'A');
    }
  }
  return valid_paths;
}

std::unordered_map<CacheKey, int, CacheKeyHash> cost_cache;

// Calculate cost between two positions
int cost_between(const std::vector<std::string> &keypad, char start, char end,
                 int links) {
  if (links == 0)
    return 1;

  CacheKey key{keypad, std::string(1, start), std::string(1, end), links};
  if (cost_cache.count(key))
    return cost_cache[key];

  int min_cost = INT_MAX;
  for (const auto &path : paths_between(keypad, start, end)) {
    min_cost = std::min(min_cost, cost(DIRECTIONAL, path, links - 1));
  }
  cost_cache[key] = min_cost;
  return min_cost;
}

// Calculate total cost for a sequence
int cost(const std::vector<std::string> &keypad, const std::string &keys,
         int links) {
  std::string sequence = "A" + keys;
  int total_cost = 0;
  for (size_t i = 0; i < sequence.length() - 1; i++) {
    total_cost += cost_between(keypad, sequence[i], sequence[i + 1], links);
  }

  return total_cost;
}

// Calculate complexity of a code
int complexity(const std::string &code, int robots) {
  std::string numeric_part = code.substr(0, code.length() - 1);
  while (numeric_part[0] == '0' && numeric_part.length() > 1) {
    numeric_part = numeric_part.substr(1);
  }
  return cost(NUMERIC, code, robots + 1) * std::stoi(numeric_part);
}

auto solve(const std::string &input) -> long {
  std::vector<std::string> codes;
  std::istringstream stream(input);
  std::string line;
  while (std::getline(stream, line)) {
    if (line.empty()) {
      continue;
    }

    codes.push_back(line);
  }

#ifdef DEBUG
  for (const auto &code : codes) {
    std::cout << code << std::endl;
  }
#endif

  auto ans = 0;
  for (const std::string &code : codes) {
    KeypadSolver solver;
    ans += solver.calculateComplexity(code);
  }

  return ans;
}
} // namespace part_one
