// Day15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>

#include "../AoCHelpers/InputHandler.h"
#include "Map.h"
#include "Unit.h"

int wmain(int argc, wchar_t* argv[]) {
   advent_of_code::InputHandler input{ argc, argv };
   Map the_map{ input };
   std::wcout << the_map << std::endl;

   unsigned int rounds{ 0 };
   while (NONE == the_map.victory_reached()) {
      if (!the_map.simulate_round()) {
         std::wcout << L"Ended early!" << std::endl;
         continue;
      }
      //std::wcout << the_map << std::endl;
      ++rounds;
   }

   std::wcout << the_map << std::endl;
   std::wcout << L"Combat ends after " << rounds << L" full rounds." << std::endl;
   UnitType victorious_unit = the_map.victory_reached();
   switch (victorious_unit) {
   case ELF:
      std::wcout << L"Elves ";
      break;
   case GOBLIN:
      std::wcout << L"Goblins ";
      break;
   }
   unsigned int remaining_health{ the_map.get_total_remaining_health() };
   std::wcout << L"win with " << remaining_health << L" total hit points left" << std::endl;
   
   std::wcout << L"Outcome: " << rounds << L" * " << remaining_health
      << L" = " << rounds * remaining_health << std::endl;

   return 0;
}
