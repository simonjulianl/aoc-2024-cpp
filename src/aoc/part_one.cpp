module;

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

export module part_one;

//#define DEBUG

export namespace part_one {

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

#ifdef DEBUG
  for (const auto &pattern : designs) {
    std::cout << pattern << std::endl;
  }
#endif

  auto canFormDesign = [&patterns](const std::string &design) -> bool {
    std::vector<bool> dp(design.length() + 1, false);
    dp[0] = true;

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
          if (matches && dp[i - pattern.length()]) {
            dp[i] = true;
            break;
          }
        }
      }
    }

    return dp[design.length()];
  };

  long ans = 0;
  for (const auto &design : designs) {
    if (canFormDesign(design)) {
      ans++;
    }
  }
  return ans;
}
} // namespace part_one
