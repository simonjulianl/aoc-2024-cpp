module;

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

export module part_one;

#define DEBUG

export namespace part_one {

auto solve(const std::string &input) -> long {
  auto stream = std::istringstream{input};
  std::stringstream ss;
  std::string line;
  while (std::getline(stream, line)) {
    if (line.empty()) {
      break;
    }

    ss << line << std::endl;
  }

  const auto &first = ss.str();

  auto before_mapping = std::unordered_map<long, std::vector<long>>{};
  auto first_stream = std::istringstream(first);
  bool first_flag = true;
  while (std::getline(first_stream, line)) {
    if (line.empty()) {
      break;
    }

    size_t delimiter_pos = line.find('|');
    auto first_part = line.substr(0, delimiter_pos);
    auto second_part = line.substr(delimiter_pos + 1);

    auto first_number = std::stol(first_part);
    auto second_number = std::stol(second_part);

    before_mapping[first_number].push_back(second_number);
  }

  // part 2
  ss.clear();
  ss.str("");
  while (std::getline(stream, line)) {
    ss << line << std::endl;
  }

  long ans = 0;
  const auto &second = ss.str();
  auto second_stream = std::istringstream(second);
  while (std::getline(second_stream, line)) {
    if (line.empty()) {
      continue;
    }

#ifdef DEBUG
    std::cout << line << std::endl;
    std::cout << "---" << std::endl;
#endif

    auto tokens =
        std::views::split(line, ',') | std::views::transform([](auto &&range) {
          return std::stol(std::string(range.begin(), range.end()));
        });

    std::vector<int> numbers(tokens.begin(), tokens.end());

    auto valid = true;
    for (size_t i = 0; i < numbers.size(); i++) {
      for (size_t j = i + 1; j < numbers.size(); j++) {
        auto before_num = numbers[i];
        auto after_num = numbers[j];

        if (before_mapping.find(after_num) != before_mapping.end()) {
          const auto &values = before_mapping[after_num];
          if (std::find(values.begin(), values.end(), before_num) !=
              values.end()) {
            valid = false;
            break;
          }
        }
      }
    }

    if (valid) {
      auto n = numbers[numbers.size() / 2];
      ans += n;
    }
  }

  return ans;
}
} // namespace part_one
