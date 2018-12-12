// Day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

#include "../AoCHelpers/InputHandler.h"

size_t get_reduced_length(const std::wstring& polymer);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  std::wstring line{ input.read_single_line() };

  std::wcout << L"Part 1: " << get_reduced_length(line) << std::endl;

  size_t minimum_length{ std::numeric_limits<size_t>::max() };
  for (wchar_t ch = L'a'; ch <= L'z'; ++ch) {
    auto wchar_case_insensitive_comparator{ [ch](const wchar_t c)
        { return ch == c || towupper(ch) == c; } };
    std::wstring copy{ line };
    copy.erase(std::remove_if(copy.begin(), copy.end(), wchar_case_insensitive_comparator), copy.end());
    size_t reduced_length = get_reduced_length(copy);

    if (reduced_length < minimum_length) {
      minimum_length = reduced_length;
    }
  }

  std::wcout << L"Part 2: " << minimum_length << std::endl;
}

size_t get_reduced_length(const std::wstring& polymer) {
  std::stack<wchar_t> shrunken_polymer;
  for (const wchar_t ch : polymer) {
    if (0 == shrunken_polymer.size()) {
      shrunken_polymer.push(ch);
      continue;
    }

    wchar_t other{ shrunken_polymer.top() };

    // If ch1 and ch2 are the same letter, but not the same case, then their towlower versions will
    // be equal, but they themselves will be inequal
    if (ch != other && towlower(ch) == towlower(other)) {
      shrunken_polymer.pop();
    } else {
      shrunken_polymer.push(ch);
    }
  }

  return shrunken_polymer.size();
}
