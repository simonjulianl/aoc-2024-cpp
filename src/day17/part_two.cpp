module;

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <queue>
#include <ranges>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_two;

#define DEBUG

export namespace part_two {

auto getInitA(std::vector<long> &programs, long currentInput, long &refAns)
    -> void {
  // need to do dfs over this
  //  if the program is X_16,X_15,X_14,...,X_0
  // look for all a0s which f(a0) output X_0
  // look for all a1s which f(a0 * 8 + a1) output X_1,X_0
  // look for all a2s which f(a0 * 8 * 8 + a1 * 8 + a2) output X_2,X_1,X_0
  if (programs.size() == 0) {
    refAns = std::min(refAns, currentInput);
    return;
  }

  long current_number = programs.back();

  for (int i = 0; i < 8; i++) {
    std::vector<long> ans;
    // find digit one by one
    long initA = currentInput * 8 + i;
    long registerA = initA;
    long registerB = 0;
    long registerC = 0;

    while (registerA != 0) {
      registerB = (registerA % 8) ^ 0b111;
      registerC = registerA >> registerB;
      registerB = registerB ^ registerC ^ 0b111;
      ans.push_back(registerB % 8);
      registerA = registerA >> 3;
    }

    if (ans.size() > 0 && ans.front() == current_number) {
      // potentially matching
      programs.pop_back();
      getInitA(programs, initA, refAns);
      programs.push_back(current_number);
    }
  }
}

auto solve(const std::string &input) -> long {
  long registerA = 0, registerB = 0, registerC = 0;
  std::vector<std::string> lines;
  std::istringstream stream(input);
  std::string line;
  std::vector<long> program;

  // This part is not important for part two, but I'll leave it for parsing
  // purposes
  while (std::getline(stream, line)) {
    if (line.starts_with("Register A:")) {
      registerA = std::stol(line.substr(12));
    } else if (line.starts_with("Register B:")) {
      registerB = std::stol(line.substr(12));
    } else if (line.starts_with("Register C:")) {
      registerC = std::stol(line.substr(12));
    } else if (line.starts_with("Program:")) {
      std::string programString = line.substr(9);
      std::istringstream programStream(programString);
      std::string token;
      while (std::getline(programStream, token, ',')) {
        program.push_back(std::stol(token));
      }
    }
  }

#ifdef DEBUG
  std::cout << registerA << " " << registerB << " " << registerC << std::endl;
  for (const auto &r : program) {
    std::cout << r << " ";
  }
#endif

  long ans = std::numeric_limits<long>::max();
  getInitA(program, 0, ans);
  return ans;
}
} // namespace part_two
