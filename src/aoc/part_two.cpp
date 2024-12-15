module;

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <ranges>
#include <regex>
#include <sstream>
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

long max_long = std::numeric_limits<long>::max();

auto getOptimalCost(long dxa, long dya, long dxb, long dyb, long px, long py) -> long {
  // This is just a system of 2 linear equations ... let A be how many times I
  // press button A, let B be how many times I press button B
  // A * dxa + B * dxb = px -- (1)
  // A * dya + B * dyb = py -- (2)

  // Transform the equations to:
  // A * dya * dxa + B * dya * dxb = px * dya -- (3)
  // A * dya * dxa + B * dxa * dyb = px * dxa -- (4)
  // B ( dya * dxb - dxa * dyb ) = px * dya - py * dxa -- (5)
  // B = (px * dya - py * dxa) / (dya * dxb - dxa * dyb) -- (6)
  // A = (px - B * dxb) / dxa -- (7)

  long B = (px * dya - py * dxa) / (dya * dxb - dxa * dyb);
  long A = (px - B * dxb) / dxa;

  if (A < 0 || B < 0 || A * dxa + B * dxb != px || A * dya + B * dyb != py) {
    return max_long;
  }

  return 3 * A + B;
}

auto solve(const std::string &input) -> long {
  const std::regex pattern(R"(Button\s(\w):\sX([+-]\d+),\sY([+-]\d+))");

  const std::regex prize_pattern(R"(.*X=(\d+), Y=(\d+).*)");

  long ans = 0;
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

    const long displacement = 10000000000000L;
    const long true_px = px + displacement;
    const long true_py = py + displacement;

    long prize_ans = getOptimalCost(dxa, dya, dxb, dyb, true_px, true_py);

    if (prize_ans != max_long) {
      ans += prize_ans;
    }
  }

  return ans;
}
} // namespace part_two
