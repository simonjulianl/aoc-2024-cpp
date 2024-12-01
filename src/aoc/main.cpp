#include <iostream>

import input;
import part_one;
import part_two;

int main() {
  std::cout << "Solution to part 1: " << part_one::solve(input::input) << std::endl;
  std::cout << "Solution to part 2: " << part_two::solve(input::input) << std::endl;
  return 0;
}