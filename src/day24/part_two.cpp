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

export module part_two;

#define DEBUG

export namespace part_two {

struct Gate {
  std::string op;
  std::string input1;
  std::string input2;
  std::string output;
};

auto checkWireIsInput(const std::string &w) -> bool {
  return w[0] == 'x' || w[0] == 'y';
}

// Heavily inspired by :
// https://github.com/vss2sn/advent_of_code/blob/master/2024/cpp/day_24b.cpp
auto solve(const std::string &input) -> long {
  std::istringstream stream(input);
  std::string line;
  std::unordered_map<std::string, std::vector<Gate>> map;
  std::vector<Gate> gates;

  auto isFirstPart = true;
  while (std::getline(stream, line)) {
    if (line.empty()) {
      isFirstPart = false;
      continue;
    }

    if (isFirstPart) {
      continue;
    } else {
      // second part
      std::regex pattern(R"((\w+)\s+(XOR|AND|OR)\s+(\w+)\s+->\s+(\w+))");
      std::smatch match;
      if (std::regex_match(line, match, pattern)) {
        std::string i1 = match[1].str();
        std::string op = match[2].str();
        std::string i2 = match[3].str();
        std::string output = match[4].str();

        auto newGate = Gate{op, i1, i2, output};
        gates.push_back(newGate);
        map[i1].push_back(newGate);
        map[i2].push_back(newGate);
      }
    }
  }

  for (auto &[wire, ops] : map) {
    std::sort(std::begin(ops), std::end(ops),
              [](const auto &a, const auto &b) { return a.op < b.op; });
  }

#ifdef DEBUG
  for (const auto &gate : gates) {
    std::cout << gate.input1 << " " << gate.op << " " << gate.input2 << " -> "
              << gate.output << std::endl;
  }
#endif

  std::set<std::string> incorrect_outputs;
  for (const auto &operation : gates) {
    const auto &w1 = operation.input1;
    const auto &w2 = operation.input2;
    const auto &out = operation.output;
    const auto &op = operation.op;

    /**
     * From:
     https://www.reddit.com/r/adventofcode/comments/1hl698z/2024_day_24_solutions/
     *  By using the following formula
        Zn = (Xn ⊕ Yn) ⊕ Cn-1
        Cn = (Xn * Yn) + (Cn-1 * (Xn ⊕ Yn))
        with C0 = (X0 * Y0)

        We can derive a series of rule.

        AND:

        AND gate can only be input to an OR gate except for z01
        AND gate cannot take other AND gate as input

        XOR:
        XOR gate can only be input to and AND/XOR gate
        XOR gate cannot take AND gate as input except for z01

        OR:
        OR gate can only be input of AND/XOR gate
        OR gate can only take AND gate as input
        (Xn ⊕ Yn) ⊕ (a + b) should always output a Zxx except for the last carry
     z45 A gate with Zxx as its output cannot directly use Xn or Yn as inputs
     exept the first bit Z00. Look for gates that do not follow those rules.
     */

    if (out[0] == 'z') {
      const auto idx = std::stoi(out.substr(1, out.size() - 1));
      if (idx == 0 || idx == 45) {
        continue;
      }
    }

    const bool is_first_input = checkWireIsInput(w1);
    const bool is_second_input = checkWireIsInput(w2);

    // This is just the subset of the rules
    if (is_first_input && !is_second_input) {
      incorrect_outputs.insert(out);
    }

    if (!is_first_input && is_second_input) {
      incorrect_outputs.insert(out);
    }

    if (op == "AND") {
      if (map[out].size() < 1 || map[out][0].op != "OR") {
        incorrect_outputs.insert(out);
      }
    } else if (op == "OR") {
      if (is_first_input || is_second_input) {
        incorrect_outputs.insert(out);
      }

      if (!map.contains(out) || map[out].size() != 2 ||
          map[out][0].op != "AND" || map[out][1].op != "XOR") {
        incorrect_outputs.insert(out);
      }
    } else if (op == "XOR") {
      if (is_first_input && out[0] == 'z') {
        incorrect_outputs.insert(out);
      }

      if (is_first_input &&
          (map[out][0].op != "AND" || map[out][1].op != "XOR")) {
        incorrect_outputs.insert(out);
      }

      if (!is_first_input && out[0] != 'z') {
        incorrect_outputs.insert(out);
      }
    }
  }

  for (const auto &op : gates) {
    if (op.input1 == "x00" || op.input1 == "x00" || op.input1 == "y00" ||
        op.input2 == "y00") {
      // first z00 doesn't have carry bit but its included here
      // c0 doesn't have or output, special rule
      incorrect_outputs.erase(op.output);
    }
  }

  for (const auto &ele : incorrect_outputs) {
    std::cout << ele << ',';
  }

  return 1;
}
} // namespace part_two
