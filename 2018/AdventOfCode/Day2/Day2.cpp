// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Box.h"

int main()
{
   wifstream input{ L"Input.txt" };
   if (!input.good())
   {
      return -1;
   }

   wstring boxName;
   vector<Box> boxes;
   do
   {
      getline(input, boxName);
      boxes.push_back(Box(boxName));
   } while (input.good());

   unsigned int count_two = count_if(boxes.begin(), boxes.end(), [](const Box& b) { return b.has_exactly_two_of_a_character(); });
   unsigned int count_three = count_if(boxes.begin(), boxes.end(), [](const Box& b) { return b.has_exactly_three_of_a_character(); });

   wcout << L"Boxes with two: " << count_two << endl;
   wcout << L"Boxes with three: " << count_three << endl;
   wcout << L"Checksum: " << count_two * count_three << endl;

   for (unsigned int i = 0; i < boxes.size(); ++i)
   {
      for (unsigned int j = i + 1; j < boxes.size(); ++j)
      {
         if (1 == boxes[i].edit_distance(boxes[j]))
         {
            wcout << L"Boxes \"" << boxes[i].get_name() << L"\" and \"" << boxes[j].get_name() << L"\" differ by only 1 letter." << endl;
         }
      }
   }

   return 0;
}
