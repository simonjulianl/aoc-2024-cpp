module;

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
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

struct Gate {
  std::string operation;
  std::string input1;
  std::string input2;
  std::string output;
};

auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::string line;
  std::unordered_map<std::string, bool> values;
  std::vector<Gate> gates;

  auto isFirstPart = true;
  while (std::getline(stream, line)) {
    if (line.empty()) {
      isFirstPart = false;
      continue;
    }

    if (isFirstPart) {
      auto pos = line.find(":");
      if (pos == std::string::npos) {
        continue;
      } else {
        std::string cable = line.substr(0, pos);
        std::string value = line.substr(pos + 2);
        values[cable] = std::stoi(value) == 1;
      }
    } else {
      // second part
      std::regex pattern(R"((\w+)\s+(XOR|AND|OR)\s+(\w+)\s+->\s+(\w+))");
      std::smatch match;
      if (std::regex_match(line, match, pattern)) {
        std::string i1 = match[1].str();
        std::string op = match[2].str();
        std::string i2 = match[3].str();
        std::string output = match[4].str();

        gates.push_back(Gate{op, i1, i2, output});
      }
    }
  }

#ifdef DEBUG
  for (const auto &gate : gates) {
    std::cout << gate.input1 << " " << gate.operation << " " << gate.input2
              << " -> " << gate.output << std::endl;
  }
#endif

  auto solvedGate = 0;
  while (solvedGate != (int)gates.size()) {
    // foreach gate, check if we can compute the value
    for (const auto &gate : gates) {
      if (values.count(gate.output) != 0) {
        continue;
      }

      // output is still unknown
      if (values.count(gate.input1) > 0 && values.count(gate.input2) > 0) {
        auto i1 = values[gate.input1];
        auto i2 = values[gate.input2];

        bool output;
        if (gate.operation == "AND") {
          output = i1 && i2;
        } else if (gate.operation == "OR") {
          output = i1 || i2;
        } else {
          output = i1 ^ i2;
        }

        values[gate.output] = output;
        solvedGate++;
      }
    }
  }

  // From my obs, max z value is z45 and its sequential (i.e. z00 - z45)
  long long multiplier = 1;
  long long ans = 0;
  for (int i = 0; i <= 45; ++i) {
    std::ostringstream oss;
    oss << "z" << (i < 10 ? "0" : "")
        << i; // Append leading zero for numbers < 10
    auto key = oss.str();
    auto value = values[key];
    if (value) {
      ans += multiplier;
    }

    multiplier <<= 1;
  }
  return ans;
}
} // namespace part_one
