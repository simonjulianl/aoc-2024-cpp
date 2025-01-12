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
#include <numeric>

export module part_one;

#define DEBUG

export namespace part_one {

auto pruneNumber(const long &number) -> long { return number % 16777216; }

auto mixNumber(const long &number, const long &secretNumber) -> long {
  return number ^ secretNumber;
}

auto getNextSecretNumber(const long &number) -> long {
  long a = pruneNumber(mixNumber(number << 6, number));
  long b = pruneNumber(mixNumber(a >> 5, a));
  long c = pruneNumber(mixNumber(b << 11, b));
  return c;
}

auto getNextIthSecretNumber(const long &number, const long &i) -> long {
  auto curr = number;
  for (auto j = 0; j < i; j++) {
    curr = getNextSecretNumber(curr);
  }

  return curr;
}

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::string line;

  std::vector<long> numbers;
  while (std::getline(stream, line)) {
    numbers.push_back(std::stol(line));
  }

  auto ans = std::accumulate(
      numbers.begin(), numbers.end(), 0L, [](long sum, const long &number) {
        return sum + getNextIthSecretNumber(number, 2000);
      });

  return ans;
}
} // namespace part_one
