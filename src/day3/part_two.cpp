module;

#include <iostream>
#include <regex>
#include <vector>

export module part_two;

export namespace part_two {

auto solve(const std::string &input) -> long {
  long ans = 0;
  std::regex pattern(R"(mul\((\d+),(\d+)\))");
  std::regex do_pattern(R"(do\(\)|don't\(\))");

  std::sregex_iterator begin(input.begin(), input.end(), pattern);
  std::sregex_iterator end;

  auto op_begin = std::sregex_iterator(input.begin(), input.end(), do_pattern);
  auto op_end = std::sregex_iterator();
  std::vector<std::smatch> ops(op_begin, op_end);
  auto op_it = ops.cbegin();

  auto should_do = true;
  for (const auto &match : std::vector<std::smatch>(begin, end)) {
    // Find the latest operator whose index doesn't exceed the current match
    auto curr_pos = match.position();

    while (op_it != ops.cend() && op_it->position() < curr_pos) {
      should_do = op_it->str() == "do()";
      ++op_it;
    }

    if (should_do) {
      long a = std::stol(match[1]);
      long b = std::stol(match[2]);
      ans += a * b;
    }
  }

  return ans;
}
} // namespace part_two