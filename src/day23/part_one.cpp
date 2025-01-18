module;

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_one;

#define DEBUG

export namespace part_one {

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::string line;

  std::unordered_map<std::string, std::unordered_set<std::string>> numbers;
  std::string firstPart, secondPart;

  while (std::getline(stream, line)) {
    auto pos = line.find("-");
    if (pos == std::string::npos) {
      continue;
    } else {
      firstPart = line.substr(0, pos);
      secondPart = line.substr(pos + 1);

      numbers[firstPart].insert(secondPart);
      numbers[secondPart].insert(firstPart);
    }
  }

  // Brute force solution, try every combination of 3 comps
  std::vector<std::string> keys;
  for (const auto &pair : numbers) {
    keys.push_back(pair.first);
  }

  auto ans = 0;

  // resource:
  // https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c
  int N = keys.size();
  constexpr int K = 3;
  std::string bitmask(K, 1); // K leading 1's
  bitmask.resize(N, 0);      // N-K trailing 0's

  std::cout << "N: " << N << std::endl;
  do {
    std::vector<std::string> combi;
    for (int i = 0; i < N; ++i) // [0..N-1] integers
    {
      if (bitmask[i]) {
        combi.push_back(keys[i]);
      }
    }

    auto com1 = combi[0];
    auto com2 = combi[1];
    auto com3 = combi[2];

    // Check if any of the com starts with t
    if (com1[0] != 't' && com2[0] != 't' && com3[0] != 't') {
      continue;
    }

    if (numbers[com1].find(com2) != numbers[com1].end() &&
        numbers[com1].find(com3) != numbers[com1].end() &&
        numbers[com2].find(com3) != numbers[com2].end()) {
      ans += 1;
    }
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

  return ans;
}
} // namespace part_one
