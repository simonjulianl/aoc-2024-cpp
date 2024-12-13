module;

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <iterator>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_one;

// #define DEBUG

export namespace part_one {

auto countDigits(long long num) {
  if (num == 0) {
    return 1;
  }

  return static_cast<int>(std::log10(num)) + 1;
}

auto solve(const std::string &input) -> long {
  auto tokens =
      std::views::split(input, ' ') | std::views::transform([](auto &&range) {
        return std::stol(std::string(range.begin(), range.end()));
      });

  std::vector<long long> numbers(tokens.begin(), tokens.end());

  int blink = 0;
  while (blink < 25) {
    std::vector<long long> new_numbers;
    for (const auto &num : numbers) {
      if (num == 0) {
        new_numbers.push_back(1);
        continue;
      }

      int num_digits = countDigits(num);
      if (num_digits % 2 == 0) {
        // split the rock into two
        long long divisor =
            static_cast<long long>(std::pow(10, num_digits / 2));
        long long first_half = num / divisor;
        long long second_half = num % divisor;

        new_numbers.push_back(first_half);
        new_numbers.push_back(second_half);
        continue;
      }

      // last case
      new_numbers.push_back(num * 2024);
    }
    numbers = new_numbers;
    blink++;
  }

  return numbers.size();
}
} // namespace part_one
