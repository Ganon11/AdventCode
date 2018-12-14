// Day14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "../AoCHelpers/Sample.h"

typedef std::vector<unsigned short> DigitVector;

DigitVector get_digits(unsigned short num);
DigitVector get_digits(const std::wstring& input);
bool generate_new_recipes(DigitVector& recipe_scores, size_t& elf_1, size_t& elf2);

void part_1(const std::wstring& input);

size_t find_digits_in_recipes(const DigitVector& digits, const DigitVector& recipe_scores);
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

DigitVector get_digits(unsigned short num) {
  DigitVector digits;
  if (0u == num) {
    digits.push_back(0);
  } else {
    unsigned short num_digits{ static_cast<unsigned short>(log10(num) + 1) };
    digits.reserve(num_digits);
    for (int digit = 0; digit < num_digits; ++digit) {
      digits.insert(digits.begin(), num % 10);
      num /= 10;
    }
  }
  return digits;
}

DigitVector get_digits(const std::wstring& input) {
  DigitVector digits;
  digits.reserve(input.size());
  for (const wchar_t ch : input) {
    digits.push_back(ch - L'0');
  }

  return digits;
}

bool generate_new_recipes(DigitVector& recipe_scores, size_t& elf_1, size_t& elf_2) {
  DigitVector digits{ get_digits(recipe_scores[elf_1] + recipe_scores[elf_2]) };
  if (digits.size() == 0 || digits.size() > 2) {
    std::wcerr << L"Unexpected result from get_digits" << std::endl;
    return false;
  }

  recipe_scores.insert(recipe_scores.end(), digits.begin(), digits.end());
  elf_1 = (elf_1 + recipe_scores[elf_1] + 1) % recipe_scores.size();
  elf_2 = (elf_2 + recipe_scores[elf_2] + 1) % recipe_scores.size();

  return true;
}

void part_1(const std::wstring& input) {
  const size_t INPUT_AS_NUMBER{ static_cast<size_t>(_wtoi(input.c_str())) };
  const size_t RECIPES_TO_CREATE{ INPUT_AS_NUMBER + 10 };
  DigitVector recipe_scores{ 3, 7 };
  recipe_scores.reserve(RECIPES_TO_CREATE);
  size_t elf_1{ 0 }, elf_2{ 1 };

  while (recipe_scores.size() < RECIPES_TO_CREATE) {
    if (!generate_new_recipes(recipe_scores, elf_1, elf_2)) {
      return;
    }
  }

  // Part 1
  std::wcout << L"Recipe scores: ";
  for (size_t index = INPUT_AS_NUMBER; index < RECIPES_TO_CREATE; ++index) {
    std::wcout << recipe_scores[index];
  }
  std::wcout << std::endl;
}

namespace {
const size_t NOT_FOUND{ std::numeric_limits<size_t>::max() };
}

size_t find_digits_in_recipes(const DigitVector& digits, const DigitVector& recipes) {
  // If there are more digits than there are recipes, the digits cannot possibly be in the recipes
  if (digits.size() > recipes.size()) {
    return NOT_FOUND;
  }

  // Check if digits are found at the end
  size_t digits_index{ 0 };
  size_t recipe_index{ recipes.size() - digits.size() };
  bool found{ true };
  while (digits_index < digits.size()) {
    if (digits[digits_index] != recipes[recipe_index]) {
      found = false;
      break;
    } else {
      ++digits_index;
      ++recipe_index;
    }
  }

  if (found) {
    return recipes.size() - digits.size();
  }

  // Check if digits are found at the end, offset by 1
  digits_index = 0;
  recipe_index = recipes.size() - digits.size() - 1;
  found = true;
  while (digits_index < digits.size()) {
    if (digits[digits_index] != recipes[recipe_index]) {
      found = false;
      break;
    } else {
      ++digits_index;
      ++recipe_index;
    }
  }

  if (found) {
    return recipes.size() - digits.size() - 1;
  }

  // Didn't find it
  return NOT_FOUND;
}

void part_2(const std::wstring& input) {
  DigitVector input_digits{ get_digits(input) };
  DigitVector recipe_scores{ 3, 7 };
  size_t elf_1{ 0 }, elf_2{ 1 };
  size_t found{ find_digits_in_recipes(input_digits, recipe_scores) };
  while (found == NOT_FOUND) {
    if (!generate_new_recipes(recipe_scores, elf_1, elf_2)) {
      return;
    }
    found = find_digits_in_recipes(input_digits, recipe_scores);
  }

  std::wcout << L"Found after " << found << L" recipes." << std::endl;
}
