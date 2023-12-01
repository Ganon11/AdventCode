#include <cctype>
#include <iostream>

#include "../helpers/input_handler.h"

namespace {
static short PART = 1;
}

bool find_string_at_index(const std::string& line, const std::string& word, const size_t index)
{
  if (word.length() + index > line.length())
  {
    return false;
  }

  for (size_t word_index = 0; word_index < word.length(); ++word_index)
  {
    if (word[word_index] != line[index + word_index])
    {
      return false;
    }
  }

  return true;
}

short find_digit_word_at_index(const std::string& line, const size_t index)
{
  if (find_string_at_index(line, "one", index))
  {
    return 1;
  }
  
  if (find_string_at_index(line, "two", index))
  {
    return 2;
  }
  
  if (find_string_at_index(line, "three", index))
  {
    return 3;
  }
  
  if (find_string_at_index(line, "four", index))
  {
    return 4;
  }
  
  if (find_string_at_index(line, "five", index))
  {
    return 5;
  }
  
  if (find_string_at_index(line, "six", index))
  {
    return 6;
  }
  
  if (find_string_at_index(line, "seven", index))
  {
    return 7;
  }
  
  if (find_string_at_index(line, "eight", index))
  {
    return 8;
  }
  
  if (find_string_at_index(line, "nine", index))
  {
    return 9;
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
