// AdventOfCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

bool add_and_check_frequency(int frequency, int& total, set<int> &seenFrequencies);

int main()
{
   wifstream input{ L"Input.txt" };
   if (!input.good())
   {
      return -1;
   }

   int total{ 0 };
   vector<int> frequencies;
   set<int> seenFrequencies;
   seenFrequencies.insert(total);
   wstring line;
   bool found{ false };
   do
   {
      getline(input, line);
      int frequency{ _wtoi(line.c_str()) };
      frequencies.push_back(frequency);

      found = add_and_check_frequency(frequency, total, seenFrequencies);
   } while (!found && input.good());

   wcout << L"First Total " << total << endl;

   if (!found)
   {
      while (true)
      {
         for (const int& frequency : frequencies)
         {
            found = add_and_check_frequency(frequency, total, seenFrequencies);
            if (found)
            {
               break;
            }
         }

         if (found)
         {
            break;
         }
      }
   }

   return 0;
}

bool add_and_check_frequency(int frequency, int& total, set<int> &seenFrequencies)
{
   total += frequency;
   if (seenFrequencies.end() != seenFrequencies.find(total))
   {
      wcout << "Repeat frequency found: " << total << endl;
      return true;
   }

   seenFrequencies.insert(total);
   return false;
}
