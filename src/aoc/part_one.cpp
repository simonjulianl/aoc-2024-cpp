module;

#include <chrono>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <ranges>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <numeric>

export module part_one;

#define DEBUG

export namespace part_one {

auto solve(const std::string &input) -> long {
  long registerA = 0, registerB = 0, registerC = 0;
  std::vector<std::string> lines;
  std::istringstream stream(input);
  std::string line;
  std::vector<long> program;

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

  auto getComboOperand = [&](long operand) -> long {
    if (operand <= 3) {
      return operand;
    } else if (operand == 4) {
      return registerA;
    } else if (operand == 5) {
      return registerB;
    } else if (operand == 6) {
      return registerC;
    }

    // invalid program
    return 0;
  };

  std::vector<long> ans;

  long instructionPointer = 0;
  while (instructionPointer < program.size()) {
    auto currentOpCode = program[instructionPointer];
    instructionPointer++;
    switch (currentOpCode) {
    case 0: {
      // adv
      auto operand1 = getComboOperand(program[instructionPointer]);
      instructionPointer++;
      registerA /= std::pow(2, operand1);
      break;
    }
    case 1: {
      // bxl
      auto operand1 = program[instructionPointer];
      instructionPointer++;
      registerB ^= operand1;
      break;
    }
    case 2: {
      // bst instruction
      auto operand1 = getComboOperand(program[instructionPointer]) % 8;
      registerB = operand1;
      instructionPointer++;
      break;
    }
    case 3: {
      // jnz
      if (registerA == 0) {
        instructionPointer++;
      } else {
        auto operand1 = program[instructionPointer];
        instructionPointer = operand1;
      }
      break;
    }
    case 4: {
      instructionPointer++;
      registerB ^= registerC;
      break;
    }
    case 5: {
      long operand1 = getComboOperand(program[instructionPointer]) % 8;
      instructionPointer++;
      ans.push_back(operand1);
      break;
    }
    case 6: {
      long operand1 = getComboOperand(program[instructionPointer]);
      instructionPointer++;
      registerB = registerA / std::pow(2, operand1);
      break;
    }
    case 7: {
      auto operand1 = getComboOperand(program[instructionPointer]);
      instructionPointer++;
      registerC = registerA / std::pow(2, operand1);
      break;
    }
    }
  }
  auto result = std::accumulate(
      ans.begin() + 1, ans.end(), std::to_string(ans[0]),
      [](const std::string &a, long b) { return a + "," + std::to_string(b); });
  std::cout << std::endl;
  std::cout << result << std::endl;

  return 0;
}
} // namespace part_one
