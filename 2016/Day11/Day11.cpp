// Day11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Floor.h"
#include "GameState.h"
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
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

namespace {
   unsigned int GetFloorNum(const std::wstring& ordinal) {
      if (ordinal == L"first") {
         return 0;
      }
      else if (ordinal == L"second") {
         return 1;
      }
      else if (ordinal == L"third") {
         return 2;
      }
      else if (ordinal == L"fourth") {
         return 3;
      }

      return 0xFFFF;
   }
}

ObjectState ConstructObjects(const std::wstring& inputFile) {
   ObjectState state;

   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return state;
   }

   std::map<std::wstring, ObjectPair> objectsSeenSoFar;
   std::wstring line;
   while (std::getline(input, line)) {
      std::wregex floorNumRe{ L"The (\\w+) floor contains" };
      std::wsmatch floorNumMatch;
      if (!std::regex_search(line, floorNumMatch, floorNumRe)) {
         std::wcerr << L"Invalid line." << std::endl;
         throw L"Invalid line.";
      }
      unsigned int floorNum{ GetFloorNum(floorNumMatch[1]) };

      std::wregex chipRe{ L"a (\\w+)-compatible microchip" };
      std::wsmatch chipMatch;
      std::wstring::const_iterator search{ std::begin(line) }, end{ std::end(line) };
      while (std::regex_search(search, end, chipMatch, chipRe)) {
         std::wstring material{ chipMatch[1] };
         auto itr = objectsSeenSoFar.find(material);
         if (std::end(objectsSeenSoFar) == itr) {
            ObjectPair newPair;
            newPair.SetChipFloor(floorNum);
            objectsSeenSoFar.insert(std::make_pair(material, newPair));
         }
         else {
            itr->second.SetChipFloor(floorNum);
         }
         search += chipMatch.position() + chipMatch.length();
      }

      std::wregex genRe{ L"a (\\w+) generator" };
      std::wsmatch genMatch;
      search = std::begin(line);
      while (std::regex_search(search, end, genMatch, genRe)) {
         std::wstring material{ genMatch[1] };

         auto itr = objectsSeenSoFar.find(material);
         if (std::end(objectsSeenSoFar) == itr) {
            ObjectPair newPair;
            newPair.SetGeneratorFloor(floorNum);
            objectsSeenSoFar.insert(std::make_pair(material, newPair));
         }
         else {
            itr->second.SetGeneratorFloor(floorNum);
         }
         search += genMatch.position() + genMatch.length();
      }
   }

   for (const auto& pair : objectsSeenSoFar) {
      state.push_back(pair.second);
   }

   return state;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Sample.txt";
   }

   //FloorState initialFloors = ConstructFloors(inputFile);
   ObjectState initialState = ConstructObjects(inputFile);
   std::unordered_set<GameState> seenStates;

   //GameState beginningState{ 0, initialFloors, 0 };
   GameState beginningState{ 0, initialState, 0 };

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
