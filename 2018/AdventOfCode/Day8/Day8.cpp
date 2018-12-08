// Day8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "TreeNode.h"

std::vector<unsigned int> parse_input(const std::wstring& line);

int main()
{
   AdventOfCode::InputHandler input;
   std::vector<unsigned int> numbers{ parse_input(input.read_single_line()) };
   std::unique_ptr<TreeNode> root{ TreeNode::make_unique(numbers) };

   std::wcout << L"Part 1: " << root->sum_metadata() << std::endl;
   std::wcout << L"Part 2: " << root->get_value() << std::endl;
   return 0;
}

std::vector<unsigned int> parse_input(const std::wstring& line)
{
   std::vector<unsigned int> numbers;
   std::wistringstream tokenStream{ line };
   std::wstring token;
   while (std::getline(tokenStream, token, L' '))
   {
      numbers.push_back(_wtoi(token.c_str()));
   }

   return numbers;
}
