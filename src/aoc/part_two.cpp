module;

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <ranges>
#include <sstream>
#include <stack>
#include <string>
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

auto countDigits(long long num) -> long long {
  if (num == 0) {
    return 1;
  }

  return static_cast<long long>(std::log10(num)) + 1;
}

std::unordered_map<std::pair<long long, long long>, long long, pair_hash> cache;

auto getAns(long long number, long long iteration) -> long long {
  if (iteration == 0) {
    return 1;
  }

  if (auto it = cache.find({number, iteration}); it != cache.end()) {
    return it->second;
  }

  long long num_digits = countDigits(number);
  long long ans = 0;
  if (number == 0) {
    ans = getAns(1, iteration - 1);
  } else if (num_digits % 2 == 0) {
    long long divisor = static_cast<long long>(std::pow(10, num_digits / 2));
    long long first_half = number / divisor;
    long long second_half = number % divisor;

    ans += getAns(first_half, iteration - 1);
    ans += getAns(second_half, iteration - 1);
  } else {
    ans += getAns(number * 2024, iteration - 1);
  }

  cache.insert({{number, iteration}, ans});
  return ans;
}

auto solve(const std::string &input) -> long long {
  auto tokens =
      std::views::split(input, ' ') | std::views::transform([](auto &&range) {
        return std::stol(std::string(range.begin(), range.end()));
      });

  std::vector<long long> numbers(tokens.begin(), tokens.end());

  long long ans = std::accumulate(
      numbers.begin(), numbers.end(), 0LL,
      [](long long acc, long long num) { return acc + getAns(num, 75); });
  return ans;
}
} // namespace part_two
