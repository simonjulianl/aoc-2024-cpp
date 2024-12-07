module;

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_two;

// #define DEBUG

export namespace part_two {

auto parse_line(const std::string &input) -> std::vector<long> {
  std::vector<long> result;
  size_t colon_pos = input.find(':');

  if (colon_pos != std::string::npos) {
    auto right_part = input.substr(colon_pos + 1);
    std::stringstream ss(right_part);
    long num;
    while (ss >> num) {
      result.push_back(num);
    }

    auto left_part = input.substr(0, colon_pos);
    result.push_back(std::stol(left_part));
  }

  return result;
}

auto can_match(const std::vector<long> &nums, long idx, long current_sum,
               long target) -> bool {
  if (static_cast<long>(nums.size()) == idx) {
    return current_sum == target;
  }

  long last_number = nums[idx];

  auto str = std::to_string(current_sum) + std::to_string(last_number);
  auto result = can_match(nums, idx + 1, current_sum * last_number, target) ||
                can_match(nums, idx + 1, current_sum + last_number, target) ||
                can_match(nums, idx + 1, std::stol(str), target);

  return result;
}

auto solve(const std::string &input) -> long {
  long ans = 0;
  for (const auto &line : input | std::views::split('\n')) {
    std::vector<long> nums =
        parse_line(std::string(std::begin(line), std::end(line)));
    long target = nums.back();
    nums.pop_back();

#ifdef DEBUG
    std::cout << "Target: " << target << std::endl;
    for (const auto &num : nums) {
      std::cout << num << " ";
    }
    std::cout << std::endl;
#endif

    // assume nums.size() >= 1
    auto result = can_match(nums, 1, nums[0], target);

#ifdef DEBUG
    std::cout << "Result: " << result << std::endl;
#endif

    if (result) {
      ans += target;
    }
  }

  return ans;
}
} // namespace part_two
