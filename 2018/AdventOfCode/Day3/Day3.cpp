// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Claim.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main()
{
   wifstream input{ L"Input.txt" };
   if (!input.good())
   {
      return -1;
   }

   wstring line;
   vector<Claim> claims;
   do
   {
      getline(input, line);
      claims.push_back(Claim(line));
   } while (input.good());

   map<Claim::Coordinate, unsigned int> fabric;

   for (const Claim& claim : claims)
   {
      for (const Claim::Coordinate& coordinate : claim.generate_coordinates())
      {
         if (fabric.end() == fabric.find(coordinate))
         {
            fabric[coordinate] = 1u;
         }
         else
         {
            ++fabric[coordinate];
         }
      }
   }

   unsigned int inches_double_claimed = count_if(fabric.begin(), fabric.end(), [](const auto& claim) { return claim.second > 1u; });

   wcout << L"Square inches claimed multiple times: " << inches_double_claimed << endl;

   for (const Claim& claim : claims)
   {
      const vector<Claim::Coordinate> coordinates{ claim.generate_coordinates() };
      if (all_of(coordinates.begin(), coordinates.end(), [fabric](const Claim::Coordinate& coordinate) { return fabric.at(coordinate) == 1u; }))
      {
         wcout << L"Claim #" << claim.get_id() << L" does not overlap with any others." << endl;
         break;
      }
   }

   return 0;
}
