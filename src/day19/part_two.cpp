module;

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

export module part_two;

//#define DEBUG

export namespace part_two {

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::string line;
  std::vector<std::string> patterns;
  std::vector<std::string> designs;
  bool firstLine = true;

  while (std::getline(stream, line)) {
    if (line.empty()) {
      continue;
    }

    if (firstLine) {
      std::string pattern;
      std::istringstream patternStream(line);
      while (std::getline(patternStream, pattern, ',')) {
        pattern.erase(remove_if(pattern.begin(), pattern.end(), isspace),
                      pattern.end());
        if (!pattern.empty()) {
          patterns.push_back(pattern);
        }
      }

      firstLine = false;
    } else {
      line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
      if (!line.empty()) {
        designs.push_back(line);
      }
    }
  }

  auto canFormDesign = [&patterns](const std::string &design) -> long {
    std::vector<long> dp(design.length() + 1, 0);
    dp[0] = 1;

    for (size_t i = 1; i <= design.length(); i++) {
      for (const auto &pattern : patterns) {
        if (pattern.length() <= i) {
          bool matches = true;

          // Check if the pattern matches the design
          for (size_t j = 0; j < pattern.length(); j++) {
            if (design[i - pattern.length() + j] != pattern[j]) {
              matches = false;
              break;
            }
          }

          // If pattern matches, check if the previous substring can be formed
          if (matches) {
            dp[i] += dp[i - pattern.length()];
          }
        }
      }
    }

#ifdef DEBUG
    for (size_t idx = 0; idx < dp.size(); ++idx) {
      std::cout << "Index " << idx << ": " << dp[idx] << std::endl;
    }
    std::cout << dp[design.length()] << std::endl;
#endif

    return dp[design.length()];
  };

  long ans = 0;
  for (const auto &design : designs) {
    auto temp = canFormDesign(design);
#ifdef DEBUG
    std::cout << "Design: " << design << " -> " << temp << std::endl;
#endif
    ans += temp;
  }
  return ans;
}
} // namespace part_two
