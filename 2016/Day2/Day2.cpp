// Day2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Direction.h"
#include "Numpad.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

InstructionList ParseInstructions(const std::wstring& inputFileName) {
   InstructionList instructions;

   std::wifstream input{ inputFileName };
   if (!input.is_open()) {
      return instructions;
   }

   do {
      std::wstring line;
      std::getline(input, line);
      DirectionList directions;
      for (const wchar_t c : line) {
         Direction d = GetDirection(c);
         if (d != DIRECTION_NONE) {
            directions.push_back(d);
         }
      }
      instructions.push_back(directions);
   } while (input.good());

   return instructions;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Input.txt";
   }

   InstructionList instructions = ParseInstructions(inputFile);

   NumpadPosition simpleNumpad{ NUMPAD_SIMPLE };
   NumpadPosition complexNumpad{ NUMPAD_COMPLEX };

   for (const auto& instruction : instructions) {
      simpleNumpad.Move(instruction);
      complexNumpad.Move(instruction);
   }

   std::wcout << L"Simple code: " << simpleNumpad.ToString() << std::endl;
   std::wcout << L"Complex code: " << complexNumpad.ToString() << std::endl;

   return 0;
}
