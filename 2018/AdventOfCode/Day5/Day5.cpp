// Day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

#include "../AoCHelpers/InputHandler.h"

using namespace std;

size_t get_reduced_length(const std::wstring& polymer);

int main()
{
   AdventOfCode::InputHandler input{ 5 };
   wstring line{ input.read_single_line() };

   wcout << L"Part 1: " << get_reduced_length(line) << endl;

   size_t minimum_length{ std::numeric_limits<size_t>::max() };
   for (wchar_t ch = L'a'; ch <= L'z'; ++ch)
   {
      wstring copy{ line };
      copy.erase(remove_if(copy.begin(), copy.end(), [ch](const wchar_t c){ return ch == c || towupper(ch) == c; }), copy.end());
      size_t reduced_length = get_reduced_length(copy);

      if (reduced_length < minimum_length)
      {
         minimum_length = reduced_length;
      }
   }

   wcout << L"Part 2: " << minimum_length << endl;
}

size_t get_reduced_length(const wstring& polymer)
{
   stack<wchar_t> shrunken_polymer;
   for (const wchar_t ch : polymer)
   {
      if (0 == shrunken_polymer.size())
      {
         shrunken_polymer.push(ch);
         continue;
      }

      wchar_t other{ shrunken_polymer.top() };

      // If ch1 and ch2 are the same letter, but not the same case, then their towlower versions will be equal, but
      // they themselves will be inequal
      if (ch != other && towlower(ch) == towlower(other))
      {
         shrunken_polymer.pop();
      }
      else
      {
         shrunken_polymer.push(ch);
      }
   }

   return shrunken_polymer.size();
}
