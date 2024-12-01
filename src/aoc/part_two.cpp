module;

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

export module part_two;

export namespace part_two {
auto solve(const std::string &input) -> long {
  auto left = std::unordered_map<long, long>();
  auto right = std::unordered_map<long, long>();
  auto ss = std::stringstream{input};

  for (std::string line; std::getline(ss, line, '\n');) {
    long a, b;
    std::istringstream iss(line);
    iss >> a >> b;
    left[a]++;
    right[b]++;
  }

  long ans = 0;
  for (auto &it : left) {
    if (right.find(it.first) != right.end()) {
      ans += right[it.first] * it.first * it.second;
    }
  }

  return ans;
}
} // namespace part_two