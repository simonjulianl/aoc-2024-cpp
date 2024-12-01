module;

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

export module input;

export namespace input {

std::string_view get_input() {
  std::string filename = "aoc/input.txt";
  std::ifstream file(filename);
  if (!file) {
    std::cerr << "Could not open file " << filename << std::endl;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

} // namespace input