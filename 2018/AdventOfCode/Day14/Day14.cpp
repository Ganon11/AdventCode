// Day14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "../AoCHelpers/Sample.h"

std::vector<unsigned short> get_digits_less_than_20(unsigned short sum);
void part_1(const std::wstring& input);

std::vector<unsigned short> get_digits(const std::wstring& input);
size_t find_digits_in_vector(
    const std::vector<unsigned short>& digits,
    const std::vector<unsigned short>& recipe_scores);
void part_2(const std::wstring& input);

int wmain(int argc, wchar_t* argv[]) {
  std::wstring input;
  if (argc < 2) {
    input = L"890691";
  } else {
    input = argv[1];
  }

  part_1(input);
  part_2(input);

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

void part_1(const std::wstring& input) {
  const size_t INPUT_AS_NUMBER{ static_cast<size_t>(_wtoi(input.c_str())) };
  const size_t RECIPES_TO_CREATE{ INPUT_AS_NUMBER + 10 };
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
  for (size_t index = INPUT_AS_NUMBER; index < RECIPES_TO_CREATE; ++index) {
    std::wcout << recipe_scores[index];
  }
  std::wcout << std::endl;
}

std::vector<unsigned short> get_digits(const std::wstring& input) {
  // This time we actually have to do it
  std::vector<unsigned short> digits;
  digits.reserve(input.size());
  for (const wchar_t ch : input) {
    digits.push_back(ch - L'0');
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

void part_2(const std::wstring& input) {
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
