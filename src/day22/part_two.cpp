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

export module part_two;

#define DEBUG

export namespace part_two {

struct TupleHash {
  template <class T1, class T2, class T3, class T4>
  std::size_t operator()(const std::tuple<T1, T2, T3, T4> &tuple) const {
    auto h1 = std::hash<T1>{}(std::get<0>(tuple));
    auto h2 = std::hash<T2>{}(std::get<1>(tuple));
    auto h3 = std::hash<T3>{}(std::get<2>(tuple));
    auto h4 = std::hash<T4>{}(std::get<3>(tuple));

    return h1 ^ h2 ^ h3 ^ h4;
  }
};

auto pruneNumber(const long &number) -> long { return number % 16777216; }

auto mixNumber(const long &number, const long &secretNumber) -> long {
  return number ^ secretNumber;
}

auto getNextSecretNumber(const long &number) -> long {
  long a = pruneNumber(mixNumber(number << 6, number));
  long b = pruneNumber(mixNumber(a >> 5, a));
  long c = pruneNumber(mixNumber(b << 11, b));
  return c;
}

auto getDeltaPriceSequence(const long &number, const long &i)
    -> std::vector<std::pair<long, long>> {
  std::vector<std::pair<long, long>> sequence;
  auto curr = number;
  for (auto j = 0; j < i - 1; j++) {
    auto prev = curr;
    curr = getNextSecretNumber(curr);
    auto diff = curr % 10 - prev % 10;
    sequence.push_back({diff, curr % 10});
  }

  return sequence;
}

auto getBananaGivenSequence(const std::vector<long> &seq,
                            const std::vector<std::pair<long, long>> &prices)
    -> long {
  // Since the seq is of length 4, let's just brute force pattern matching
  for (auto i = 0; i < prices.size() - 3; i++) {
    if (prices[i].first == seq[0] && prices[i + 1].first == seq[1] &&
        prices[i + 2].first == seq[2] && prices[i + 3].first == seq[3]) {
      return prices[i + 3].second;
    }
  }

  return -1;
}

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::string line;

  std::vector<long> numbers;
  while (std::getline(stream, line)) {
    numbers.push_back(std::stol(line));
  }

  std::unordered_map<std::tuple<long, long, long, long>, long, TupleHash> cache;
  // Precompute every seq of 4 numbers
  for (const auto &number : numbers) {
    auto priceSeq = getDeltaPriceSequence(number, 2000);
    // Check for any possible sequence of 4 in the priceSeq
    std::unordered_set<std::tuple<long, long, long, long>, TupleHash> visited;
    for (auto i = 0; i < priceSeq.size() - 3; i++) {
      std::tuple<long, long, long, long> sequence =
          std::make_tuple(priceSeq[i].first, priceSeq[i + 1].first,
                          priceSeq[i + 2].first, priceSeq[i + 3].first);
      auto banana = priceSeq[i + 3].second;
      if (visited.find(sequence) == visited.end()) {
        cache[sequence] += banana;
        visited.insert(sequence);
      }
    }

#ifdef DEBUG
    std::cout << "Finish processing number: " << number << std::endl;
#endif
  }

  auto ans = std::max_element(
      cache.begin(), cache.end(),
      [](const auto &a, const auto &b) { return a.second < b.second; });

  return ans->second;
}
} // namespace part_two
