module;

#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

export module part_one;

// #define DEBUG

export namespace part_one {

auto solve(const std::string &input) -> long {
  long ans = 0;
  std::regex pattern(R"(mul\((\d+),(\d+)\))");

  std::sregex_iterator begin(input.begin(), input.end(), pattern);
  std::sregex_iterator end;

  for (const auto &match : std::vector<std::smatch>(begin, end)) {
        long a = std::stol(match[1]);
        long b = std::stol(match[2]);
        ans += a * b;
  }

  return ans;
}
} // namespace part_one
