#include <cctype>
#include <iostream>
#include <vector>

#include "input_handler.h"
#include "cxxopts.hpp"

namespace {
static std::vector<std::string> DIGIT_WORDS{
  "zero",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};
}

short find_digit_word_at_index(const std::string& line, const size_t index)
{
  for (size_t word_index = 0; word_index < DIGIT_WORDS.size(); ++word_index)
  {
    const std::string& word{ DIGIT_WORDS[word_index] };
    if (line.compare(index, word.length(), word) == 0)
    {
      return word_index;
    }
  }

  return -1;
}

short find_first_digit(const std::string& line, const bool include_words = false)
{
  for (size_t index = 0; index < line.length(); ++index)
  {
    if (isdigit(line[index]))
    {
      return line[index] - '0';
    }

    if (include_words)
    {
      short word_digit = find_digit_word_at_index(line, index);
      if (word_digit != -1)
      {
        return word_digit;
      }
    }
  }

  return -1;
}

short find_last_digit(const std::string& line, const bool include_words = false)
{
  for (size_t index = line.length() - 1; index > 0; --index)
  {
    if (isdigit(line[index]))
    {
      return line[index] - '0';
    }

    if (include_words)
    {
      short word_digit = find_digit_word_at_index(line, index);
      if (word_digit != -1)
      {
        return word_digit;
      }
    }
  }

  if (isdigit(line[0]))
  {
    return line[0] - '0';
  }

  if (include_words)
  {
    short word_digit = find_digit_word_at_index(line, 0);
    if (word_digit != -1)
    {
      return word_digit;
    }
  }

  return -1;
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d1", "Day 1 of Advent of Code");

  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("h,help", "Print usage")
  ;

  auto result = options.parse(argc, argv);
  if (result.count("help"))
  {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (!result.count("filename"))
  {
    return -1;
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  int total_part_1{ 0 };
  int total_part_2{ 0 };
  for (const auto& line : input.read_all_lines())
  {
    total_part_1 += (find_first_digit(line) * 10) + find_last_digit(line);
    total_part_2 += (find_first_digit(line, true) * 10) + find_last_digit(line, true);
  }

  std::cout << "Calibration sum (p1): " << total_part_1 << std::endl;
  std::cout << "Calibration sum (p2): " << total_part_2 << std::endl;
  return 0;
}
