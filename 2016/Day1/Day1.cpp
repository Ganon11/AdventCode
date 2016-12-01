// Day1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Instruction.h"
#include "Player.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<Instruction> ParseInstructions(const std::wstring& inputFileName) {
   std::vector<Instruction> instructions;

   std::wifstream input{ inputFileName };
   if (!input.is_open()) {
      return instructions;
   }

   do {
      std::wstring token;
      input >> token;
      if (*token.crbegin() == L',') {
         token.pop_back();
      }

      wchar_t rotationText = token[0];
      std::wstring blocksText = token.substr(1);

      instructions.push_back(Instruction(rotationText, blocksText));
   } while (input.good());

   return instructions;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Day1Input.txt";
   }

   auto instructions = ParseInstructions(inputFile);
   Player p;

   for (const auto& instruction : instructions) {
      //std::wcout << L"Instruction: Rotate " << instruction.getRotation() << L", walk " << instruction.getBlocks() << std::endl;
      p.FollowInstruction(instruction);
   }

   Position finalPosition{ p.getPosition() };
   int distance{ finalPosition.getX() + finalPosition.getY() };

   std::wcout << "Final position: (" << finalPosition.getX() << L", " << finalPosition.getY() << L")" << std::endl;
   std::wcout << "Final distance: " << distance << std::endl;

   return 0;
}
