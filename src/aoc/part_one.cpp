module;

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

export module part_one;

//#define DEBUG

export namespace part_one {

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::string line;

  auto ans = 0;
  int max_diff = 3, min_diff = 1;
  while (std::getline(stream, line)) {
    std::istringstream line_stream(line);
    long curr = -1, prev_number = -1, sign = 0;

    bool valid = true;
    while (line_stream >> curr) {
      if (prev_number == -1) {
        prev_number = curr;
        continue;
      }

      long diff = curr - prev_number;
      if (std::abs(diff) >= min_diff && std::abs(diff) <= max_diff) {
        prev_number = curr;
        if (sign == 0) {
          sign = sgn(diff);
        } else {
          // sign is already known
          auto new_sign = sgn(diff);
          if (sign != new_sign) {
            valid = false;
            break;
          }
        }
      } else {
        valid = false;
        break;
      }
    }

#ifdef DEBUG
    std::cout << "line: " << line << " valid: " << valid << std::endl;
#endif

    if (valid) {
      ans++;
    }
  }

  return ans;
}
} // namespace part_one
