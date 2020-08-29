// Day4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Room.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<Room> GetRooms(const std::wstring& inputFile) {
   std::vector<Room> rooms;
   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return rooms;
   }

   do {
      std::wstring roomText;
      input >> roomText;
      rooms.push_back(Room(roomText));
   } while (input.good());

   return rooms;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"sample.txt";
   }

   auto rooms = GetRooms(inputFile);
   int sum = 0;
   for (const auto& room : rooms) {
      if (room.IsValid()) {
         sum += room.GetSectorId();

         std::wstring name{ room.GetDecryptedName() };
         if (name == L"northpole object storage") {
            std::wcout << L"Target acquired: sector " << room.GetSectorId() << std::endl;
         }
      }
   }

   std::wcout << L"Sector ID Total: " << sum << std::endl;
   return 0;
}
