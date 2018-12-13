// Day13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "../AoCHelpers/InputHandler.h"
#include "Position.h"
#include "Track.h"

int wmain(int argc, wchar_t* argv[])
{
  advent_of_code::InputHandler input{ argc, argv };
  Track t{ input };

  unsigned int tick{ 0 };
  Position first_crash{ NONE_POSITION };

  while (true) {
    std::wcout << L"Tick " << tick << L", carts left " << t.num_carts() << std::endl;
    std::wcout << t << std::endl;
    Position crash{ t.get_crash_position() };
    if (crash != NONE_POSITION) {
      std::wcout << L"CRASH! at " << crash;

      t.remove_carts(crash);

      if (first_crash == NONE_POSITION) {
        first_crash = crash;
      }

      std::wcout << L", Carts left: " << t.num_carts() << std::endl;
    }

    if (t.num_carts() <= 1) {
      break;
    }

    t.tick();
    ++tick;
  }

  std::wcout << L"One cart left at tick " << tick << L" at position " << t.first_cart_position()
      << std::endl;
  std::wcout << t << std::endl;

  return 0;
}
