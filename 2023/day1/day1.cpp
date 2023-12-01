#include <cctype>
#include <iostream>
#include <vector>

#include "../helpers/input_handler.h"

namespace {
static short PART = 1;
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

short find_first_digit(const std::string& line)
{
  for (size_t index = 0; index < line.length(); ++index)
  {
    if (isdigit(line[index]))
    {
      return line[index] - '0';
    }

    if (PART == 2)
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

short find_last_digit(const std::string& line)
{
  for (size_t index = line.length() - 1; index > 0; --index)
  {
    if (isdigit(line[index]))
    {
      return line[index] - '0';
    }

    if (PART == 2)
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

  if (PART == 2)
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
  advent_of_code::InputHandler input{ argc, argv };
  std::string part;
  if (input.get_argument("-p", part))
  {
    if (part == "1")
      ::PART = 1;
    else if (part == "2")
      ::PART = 2;
  }

  int total{ 0 };
  for (const auto& line : input.read_all_lines())
  {
    int first = find_first_digit(line);
    int last = find_last_digit(line);

    total += (first * 10) + last;
  }

  std::cout << "Calibration sum: " << total << std::endl;
  return 0;
}
