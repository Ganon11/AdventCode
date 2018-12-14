// Day14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "../AoCHelpers/Sample.h"

std::vector<unsigned short> get_digits_less_than_20(unsigned short sum);
void part_1(size_t input);

std::vector<unsigned short> get_digits(size_t num);
size_t find_digits_in_vector(
    const std::vector<unsigned short>& digits,
    const std::vector<unsigned short>& recipe_scores);
void part_2(size_t input);

int wmain(int argc, wchar_t* argv[]) {
  if (argc > 1 && 0 == _wcsicmp(L"-use-sample", argv[1])) {
    advent_of_code::sample::set_use_sample(true);
  }

  const size_t PUZZLE_INPUT{ advent_of_code::sample::should_use_sample() ? 51589u : 890691u };
  part_1(PUZZLE_INPUT);
  part_2(PUZZLE_INPUT);

  return 0;
}

std::vector<unsigned short> get_digits_less_than_20(unsigned short sum) {
  // There's only so many possibilities (0 through 18)
  std::vector<unsigned short> digits;
  if (sum > 18) {
    std::wcerr << L"Unexpected input - sum is " << sum << std::endl;
    return digits;
  }

  if (sum > 9) {
    digits.reserve(2);
    digits.push_back(1);
    sum -= 10;
  } else {
    digits.reserve(1);
  }

  digits.push_back(sum);

  return digits;
}

void part_1(const size_t input) {
  const size_t RECIPES_TO_CREATE{ input + 10 };
  std::vector<unsigned short> recipe_scores{ 3, 7 };
  recipe_scores.reserve(RECIPES_TO_CREATE);
  size_t elf_1{ 0 }, elf_2{ 1 };

  while (recipe_scores.size() < RECIPES_TO_CREATE) {
    std::vector<unsigned short> digits{
        get_digits_less_than_20(recipe_scores[elf_1] + recipe_scores[elf_2]) };
    if (digits.size() == 0 || digits.size() > 2) {
      std::wcerr << L"Unexpected result from get_digits" << std::endl;
      return;
    }

    recipe_scores.insert(recipe_scores.end(), digits.begin(), digits.end());
    elf_1 = (elf_1 + recipe_scores[elf_1] + 1) % recipe_scores.size();
    elf_2 = (elf_2 + recipe_scores[elf_2] + 1) % recipe_scores.size();
  }

  // Part 1
  std::wcout << L"Recipe scores: ";
  for (size_t index = input; index < RECIPES_TO_CREATE; ++index) {
    std::wcout << recipe_scores[index];
  }
  std::wcout << std::endl;
}

std::vector<unsigned short> get_digits(size_t num) {
  // This time we actually have to do it
  std::vector<unsigned short> digits;
  unsigned short num_digits{ static_cast<unsigned short>(log10(num) + 1) };
  digits.reserve(num_digits);
  for (int digit = 0; digit < num_digits; ++digit) {
    digits.insert(digits.begin(), num % 10);
    num /= 10;
  }

  return digits;
}

size_t find_digits_in_vector(
    const std::vector<unsigned short>& digits,
    const std::vector<unsigned short>& recipe_scores) {
  if (digits.size() > recipe_scores.size()) {
    return std::numeric_limits<size_t>::max();
  }

  // digits are either at the very end of the recipe_scores, or off by 1
  size_t digits_index{ 0 };
  size_t recipe_index{ recipe_scores.size() - digits.size() };
  bool found{ true };
  while (digits_index < digits.size()) {
    if (digits[digits_index] != recipe_scores[recipe_index]) {
      found = false;
      break;
    } else {
      ++digits_index;
      ++recipe_index;
    }
  }

  if (found) {
    return recipe_scores.size() - digits.size();
  }

  digits_index = 0;
  recipe_index = recipe_scores.size() - digits.size() - 1;
  found = true;
  while (digits_index < digits.size()) {
    if (digits[digits_index] != recipe_scores[recipe_index]) {
      found = false;
      break;
    } else {
      ++digits_index;
      ++recipe_index;
    }
  }

  if (found) {
    return recipe_scores.size() - digits.size() - 1;
  }

  return std::numeric_limits<size_t>::max();
}

void part_2(size_t input) {
  std::vector<unsigned short> input_digits{ get_digits(input) };

  // Check
  std::wcout << L"Input digits: ";
  for (size_t index = 0; index < input_digits.size(); ++index) {
    std::wcout << input_digits[index] << L' ';
  }
  std::wcout << std::endl;

  std::vector<unsigned short> recipe_scores{ 3, 7 };
  size_t elf_1{ 0 }, elf_2{ 1 };

  size_t found{ find_digits_in_vector(input_digits, recipe_scores) };

  while (found == std::numeric_limits<size_t>::max()) {
    std::vector<unsigned short> digits{ get_digits_less_than_20(recipe_scores[elf_1] + recipe_scores[elf_2]) };
    if (digits.size() == 0 || digits.size() > 2) {
      std::wcerr << L"Unexpected result from get_digits" << std::endl;
      return;
    }

    recipe_scores.insert(recipe_scores.end(), digits.begin(), digits.end());
    elf_1 = (elf_1 + recipe_scores[elf_1] + 1) % recipe_scores.size();
    elf_2 = (elf_2 + recipe_scores[elf_2] + 1) % recipe_scores.size();
    found = find_digits_in_vector(input_digits, recipe_scores);
  }

  std::wcout << L"Found after " << found << L" recipes." << std::endl;
}
