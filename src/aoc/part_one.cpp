module;

#include <chrono>
#include <iostream>
#include <iterator>
#include <limits>
#include <ranges>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module part_one;

// #define DEBUG

export namespace part_one {

auto solve(const std::string &input) -> long {
  std::regex pattern(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");
  std::smatch match;
  std::vector<std::pair<std::pair<long, long>, std::pair<long, long>>> robots;

  for (const auto &line : input | std::views::split('\n')) {
    auto str = std::string(line.begin(), line.end());
    if (std::regex_match(str, match, pattern)) {
      long px = std::stol(match[1]);
      long py = std::stol(match[2]);
      long vx = std::stol(match[3]);
      long vy = std::stol(match[4]);
      robots.push_back({{px, py}, {vx, vy}});
    }
  }

  long wide = 101, tall = 103, time = 100;
  for (auto &robot : robots) {
    robot.first.first += time * robot.second.first;
    robot.first.second += time * robot.second.second;

    robot.first.first %= wide;
    robot.first.second %= tall;

    if (robot.first.first < 0) {
      robot.first.first += wide;
    }

    if (robot.first.second < 0) {
      robot.first.second += tall;
    }
  }

  long first_quadrant = 0, second_quadrant = 0, third_quadrant = 0,
       fourth_quadrant = 0;

  for (const auto &robot : robots) {
    if (robot.first.first < wide / 2 && robot.first.second < tall / 2) {
      first_quadrant++;
    } else if (robot.first.first > wide / 2 && robot.first.second < tall / 2) {
      second_quadrant++;
    } else if (robot.first.first < wide / 2 && robot.first.second > tall / 2) {
      third_quadrant++;
    } else if (robot.first.first > wide / 2 && robot.first.second > tall / 2) {
      fourth_quadrant++;
    }
  }

  std::cout << first_quadrant << " " << second_quadrant << " " << third_quadrant
            << " " << fourth_quadrant << std::endl;

  return first_quadrant * second_quadrant * third_quadrant * fourth_quadrant;
}
} // namespace part_one
