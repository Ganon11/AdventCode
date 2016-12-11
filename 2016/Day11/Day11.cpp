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

std::vector<Floor> ConstructFloors(const std::wstring& inputFile) {
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

   std::vector<Floor> initialFloors = ConstructFloors(inputFile);
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
