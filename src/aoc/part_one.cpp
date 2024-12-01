module;

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

export module part_one;

export namespace part_one {

auto solve(const std::string &input) -> long {
  auto left = std::vector<long>();
  auto right = std::vector<long>();
  auto ss = std::stringstream{input};

  for (std::string line; std::getline(ss, line, '\n');) {
    long a, b;
    std::istringstream iss(line);
    iss >> a >> b;
    left.push_back(a);
    right.push_back(b);
  }

  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  long ans = 0;
  for (size_t i = 0; i < left.size(); i++) {
    ans += std::abs(left[i] - right[i]);
  }

  return ans;
}
} // namespace part_one
