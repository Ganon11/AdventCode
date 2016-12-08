// Day8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Instruction.h"
#include "Screen.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

InstructionList GetInstructions(const std::wstring& inputFile) {
   InstructionList list;

   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return list;
   }

   do {
      std::wstring line;
      std::getline(input, line);
      list.push_back(MakeInstruction(line));
   } while (input.good());

   return list;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Sample.txt";
   }

   auto instructions = GetInstructions(inputFile);
   Screen screen{ 6, 50 };

   screen.FollowInstructionList(instructions);
   screen.Print();

   size_t pixelCount{ screen.PixelCount() };
   std::wcout << L"Total lit pixels: " << pixelCount << std::endl;

   return 0;
}
