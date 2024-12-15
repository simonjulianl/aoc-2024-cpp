module;

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <iterator>
#include <queue>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <limits>

export module part_one;

//#define DEBUG

export namespace part_one {

auto solve(const std::string &input) -> long {
  const std::regex pattern(R"(Button\s(\w):\sX([+-]\d+),\sY([+-]\d+))");

  const std::regex prize_pattern(R"(.*X=(\d+), Y=(\d+).*)");

  long ans = 0;
  long max_long = std::numeric_limits<long>::max();
  for (const auto &line : input | std::views::split(std::string("\n\n"))) {
    // For each prize
    std::smatch matches;
    long dxa = 0, dya = 0, dxb = 0, dyb = 0, px = 0, py = 0;
    for (const auto &group : line | std::views::split('\n')) {
      const auto group_str = std::string(group.begin(), group.end());
      if (std::regex_match(group_str, matches, pattern)) {
        const std::string &button = matches[1].str();
        const std::string &x_value = matches[2].str();
        const std::string &y_value = matches[3].str();

        if (button == "A") {
          dxa = std::stol(x_value);
          dya = std::stol(y_value);
        } else if (button == "B") {
          dxb = std::stol(x_value);
          dyb = std::stol(y_value);
        }
        continue;
      }

      if (std::regex_match(group_str, matches, prize_pattern)) {
        px = std::stol(matches[1].str());
        py = std::stol(matches[2].str());
      }
    }

    // Solve the problem
#ifdef DEBUG
    std::cout << "dxa: " << dxa << ", dya: " << dya << " | dxb: " << dxb
              << ", dyb: " << dyb << std::endl
              << "Solving for " << px << ", " << py << std::endl;
#endif
    // the qn says that  A + B <= 100... Hence, I can just brute force it lmao
    long prize_ans = max_long;
    long a_cost = 3, b_cost = 1;
    for (long a = 0; a <= 100; a++) {
      for (long b = 0; b <= 100; b++) {
        if (dxa * a + dxb * b == px && dya * a + dyb * b == py) {
          prize_ans = std::min(prize_ans, a * 3 + b);
        }
      }
    }

    if (prize_ans != max_long) {
      ans += prize_ans;
    }
  }

  return ans;
}
} // namespace part_one
