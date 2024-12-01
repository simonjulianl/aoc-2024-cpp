#include <iostream>
#include <string_view>

import input;
import part_one;
import part_two;

int main() {
  std::string_view i = input::get_input();
  std::cout << "Solution to part 1: " << part_one::solve(i) << std::endl;
  std::cout << "Solution to part 2: " << part_two::solve(i) << std::endl;
  return 0;
}