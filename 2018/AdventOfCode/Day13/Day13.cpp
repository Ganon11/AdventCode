// Day13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <set>

#include "../AoCHelpers/InputHandler.h"
#include "Position.h"
#include "Track.h"

int wmain(int argc, wchar_t* argv[])
{
  advent_of_code::InputHandler input{ argc, argv };
  Track t{ input };
  Position first_crash{ NONE_POSITION };

  //std::wcout << L"Tick " << tick << L", carts left " << t.num_carts() << std::endl;
  //std::wcout << t << std::endl;

  while (t.num_carts() > 1) {
    Position crash{ t.tick() };

    if (crash != NONE_POSITION && first_crash == NONE_POSITION) {
      first_crash = crash;
    }

    //std::wcout << L"Tick " << tick << L", carts left " << t.num_carts() << std::endl;
    //std::wcout << t << std::endl;
  }

  std::wcout << L"One cart left at position " << t.first_cart_position() << std::endl;
  //std::wcout << t << std::endl;

  return 0;
}
