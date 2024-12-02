module;

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

export module part_two;

export namespace part_two {

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

auto is_valid(std::vector<long> v) -> bool {
  std::vector<long> diffs;
  std::adjacent_difference(v.begin(), v.end(), std::back_inserter(diffs));
  // Remove the first element since it's not a diff with prev element
  diffs.erase(diffs.begin());

  long max_diff = 3, min_diff = 1;
  size_t positive_diff_within_range =
      std::count_if(diffs.begin(), diffs.end(), [&](long diff) {
        return min_diff <= diff && diff <= max_diff;
      });

  size_t negative_diff_within_range =
      std::count_if(diffs.begin(), diffs.end(), [&](long diff) {
        return min_diff <= -diff && -diff <= max_diff;
      });

  if (positive_diff_within_range == diffs.size() || negative_diff_within_range == diffs.size()) {
    return true;
  }

  return false;
}

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::string line;

  auto ans = 0;
  int max_diff = 3, min_diff = 1;
  while (std::getline(stream, line)) {
    auto valid = false;
    std::vector<long> row;
    std::istringstream line_stream(line);

    long curr;
    while (line_stream >> curr) {
      row.push_back(curr);
    }

    if (is_valid(row)) {
      valid = true;
    } else {
      for (size_t i = 0; i < row.size(); i++) {
        auto deleted_element = row[i];
        row.erase(row.begin() + i);
        if (is_valid(row)) {
          valid = true;
          break;
        }
        row.insert(row.begin() + i, deleted_element);
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
} // namespace part_two