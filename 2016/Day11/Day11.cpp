// Day11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Floor.h"
#include "GameState.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

FloorState ConstructFloors(const std::wstring& inputFile) {
   std::vector<Floor> floors;

   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return floors;
   }

   std::wstring line;
   while (std::getline(input, line)) {
      floors.push_back(Floor{ line });
   }

   return floors;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Sample.txt";
   }

   //GameState test1{}
   FloorState floor1 = {
      Floor{ 0, {
         Object{ OBJECT_GENERATOR, L"H" }
      } },
      Floor{ 1,{
         Object{ OBJECT_CHIP, L"H" }
      } },
      Floor{ 2,{
         Object{ OBJECT_CHIP, L"L" },
         Object{ OBJECT_GENERATOR, L"L"}
      } },
      Floor{ 3, {} },
   };

   FloorState floor2 = {
      Floor{ 0,{
         Object{ OBJECT_GENERATOR, L"L" }
      } },
      Floor{ 1,{
         Object{ OBJECT_CHIP, L"L" }
      } },
      Floor{ 2,{
         Object{ OBJECT_CHIP, L"H" },
         Object{ OBJECT_GENERATOR, L"H" }
      } },
      Floor{ 3,{} },
   };

   GameState state1{ 0, floor1, 10 }, state2{ 0, floor2, 10 };
   if (state1 == state2) {
      std::wcout << L"Eq." << std::endl;
   }
   else {
      std::wcout << L"Neq." << std::endl;
   }

   FloorState initialFloors = ConstructFloors(inputFile);
   std::unordered_set<GameState> seenStates;

   GameState beginningState{ 0, initialFloors, 0 };

   std::vector<GameState> moves;
   moves.push_back(beginningState);

   std::unordered_set<unsigned int> seenMoveCounts;

   do {
      GameState state{ moves[0] };
      moves.erase(std::begin(moves));
      seenStates.insert(state);

      unsigned int moveCount{ state.GetMoveCount() };
      if (state.IsFinalState()) {
         std::wcout << "Found it in " << moveCount << L" moves." << std::endl;
         break;
      }

      if (std::end(seenMoveCounts) == std::find(std::begin(seenMoveCounts), std::end(seenMoveCounts), moveCount)) {
         std::wcout << L"Now processing moves at " << moveCount << std::endl;
         std::wcout << L"Current possible move count: " << moves.size() << std::endl;
         seenMoveCounts.insert(moveCount);
      }

      std::vector<GameState> nextMoves{ state.GetMoves() };
      for (const GameState& gs : nextMoves) {
         if (std::end(seenStates) == std::find(std::begin(seenStates), std::end(seenStates), gs)) {
            // Haven't seen it before
            moves.push_back(gs);
         }
      }
   } while (moves.size() > 0);

   return 0;
}
