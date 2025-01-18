module;

#include <algorithm>
#include <functional>
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

export module part_two;

#define DEBUG

export namespace part_two {

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

  // max clique problem
  std::vector<std::string> candidates;
  for (const auto &pair : numbers) {
    candidates.push_back(pair.first);
  }
  std::sort(candidates.begin(), candidates.end());

  std::vector<std::string> bestClique;
  std::vector<std::string> currentClique;

  std::function<void(const std::vector<std::string> &)> search;

  search = [&](const std::vector<std::string> &candidates) {
    if (currentClique.size() > bestClique.size()) {
      bestClique = currentClique;
    }

    for (const auto &v : candidates) {
      currentClique.push_back(v);

      std::vector<std::string> newCandidates;
      for (const auto &u : candidates) {
        if (u <= v)
          continue; // prevent duplicate

        bool isConnected = true;
        for (const auto &c : currentClique) {
          if (numbers.at(u).count(c) == 0) {
            isConnected = false;
            break;
          }
        }

        if (isConnected) {
          newCandidates.push_back(u);
        }
      }

      // backtrack search
      search(newCandidates);
      currentClique.pop_back();
    }
  };

  search(candidates);

  std::string result = std::accumulate(
      bestClique.begin(), bestClique.end(), std::string(),
      [](const std::string &a, const std::string &b) { return a + "," + b; });

  result.erase(0, 1);
  std::cout << "Best clique: " << result << std::endl;
  return bestClique.size();
}
} // namespace part_two
