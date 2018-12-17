// Day13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <set>

#include "../AoCHelpers/InputHandler.h"
#include "../AoCHelpers/Position.h"
#include "Track.h"

int wmain(int argc, wchar_t* argv[])
{
  advent_of_code::InputHandler input{ argc, argv };
  Track t{ input };
  advent_of_code::Position first_crash{ advent_of_code::NONE_POSITION };

  while (t.num_carts() > 1) {
    advent_of_code::Position crash{ t.tick() };

    if (crash != advent_of_code::NONE_POSITION && first_crash == advent_of_code::NONE_POSITION) {
      first_crash = crash;
    }
  }

  std::wcout << L"First crash is at " << first_crash << std::endl;
  std::wcout << L"One cart left at position " << t.first_cart_position() << std::endl;

  return 0;
}
