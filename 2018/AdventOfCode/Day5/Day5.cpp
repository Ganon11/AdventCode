// Day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "../AoCHelpers/InputHandler.h"

using namespace std;

size_t process_unit_reactions(std::wstring& polymer);
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
         wcout << L"Removing " << ch << L" results in shorter string" << endl;
      }
   }

   wcout << L"Part 2: " << minimum_length << endl;
}

size_t process_unit_reactions(std::wstring& polymer)
{
   wstring shrunken_polymer;
   bool found_reaction = false;
   size_t original_length{ polymer.length() };
   for (size_t i = 0; i < (original_length - 2); ++i)
   {
      wchar_t ch1{ polymer[i] };
      wchar_t ch2{ polymer[i + 1] };

      // If ch1 and ch2 are the same letter, but not the same case, then their towlower versions will be equal, but
      // they themselves will be inequal
      if (towlower(ch1) == towlower(ch2) && ch1 != ch2)
      {
         found_reaction = true;
         shrunken_polymer = polymer.substr(0, i) + polymer.substr(i + 2);
         break;
      }
   }

   if (!found_reaction)
   {
      // Still need to check last pair
      wchar_t ch1{ polymer[original_length - 2] };
      wchar_t ch2{ polymer[original_length - 1] };

      // If ch1 and ch2 are the same letter, but not the same case, then their towlower versions will be equal, but
      // they themselves will be inequal
      if (towlower(ch1) == towlower(ch2) && ch1 != ch2)
      {
         found_reaction = true;
         shrunken_polymer = polymer.substr(0, original_length - 2);
      }
   }

   if (found_reaction)
   {
      polymer = shrunken_polymer;
   }
   return polymer.length();
}

size_t get_reduced_length(const wstring& polymer)
{
   wstring polymer_copy{ polymer };
   int iterations{ 0 };
   size_t old_size{ polymer_copy.length() };
   size_t new_size{ process_unit_reactions(polymer_copy) };
   while (old_size != new_size)
   {
      ++iterations;
      old_size = new_size;
      new_size = process_unit_reactions(polymer_copy);
   }

   return new_size;
}
