// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "../AoCHelpers/InputHandler.h"
#include "Grid.h"

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  int signed_grid_number{ _wtoi(input.read_single_line().c_str()) };
  unsigned int grid_serial_number{ static_cast<unsigned int>(signed_grid_number) };
  Grid g{ grid_serial_number };
  Coordinate c{ g.get_max_three_by_three() };

  std::wcout << L"Max Power found at (" << c.m_x << L"," << c.m_y << L")" << std::endl;

  Coordinate c2{ g.get_max_square() };

  std::wcout << L"Max Power found at (" << c2.m_x << L"," << c2.m_y << L"," << c2.m_square_size
      << L")" << std::endl;

  return 0;
}
