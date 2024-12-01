#include <iostream>
#include <string_view>

import input;
import part_one;
import part_two;

auto main() -> int {
  std::string i = input::get_input();
  long part_one_ans = part_one::solve(i);
  long part_two_ans = part_two::solve(i);

  std::cout << std::endl << "Solution to part 1: " << part_one_ans << std::endl;
  std::cout << "Solution to part 2: " << part_two_ans << std::endl;
  return 0;
}