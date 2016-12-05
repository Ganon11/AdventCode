// Day5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <fstream>
#include <iostream>
#include <string>

#include "md5.h"

bool IsInterestingHash(const std::string& hash) {
   for (int i = 0; i < 5; ++i) {
      if (hash[i] != '0') return false;
   }

   return true;
}

size_t GetNumber(const char c) {
   switch (c) {
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      return static_cast<size_t>(c - '0');
   case 'a':
   case 'b':
   case 'c':
   case 'd':
   case 'e':
   case 'f':
      return static_cast<size_t>(c - 'a' + 10);
   default:
      return 0xFFFF;
   }
}

std::string GetHash(const std::string& doorId, const unsigned int index) {
   std::string hashInput{ "" };
   hashInput.append(doorId);
   char buffer[BUFSIZ];
   _itoa(index, buffer, 10);
   hashInput.append(buffer);

   return md5(hashInput);
}

char FindInterestingCharacter_Part1(const std::string& doorId, unsigned int& index) {
   do {
      std::string digest = GetHash(doorId, index++);
      if (IsInterestingHash(digest)) {
         return digest[5];
      }
   } while (true);
}

void DoPart1(const std::string& doorId) {
   char password[9] = "________";
   unsigned int index = 0;
   std::cout << "Door code: " << password << std::endl;

   for (size_t i = 0; i < 8; ++i) {
      char c = FindInterestingCharacter_Part1(doorId, index);
      password[i] = c;
      std::cout << "Door code: " << password << std::endl;
   }
}

void FindInterestingCharacter_Part2(const std::string& doorId, unsigned int& index, char& c, size_t& position) {
   do {
      std::string digest = GetHash(doorId, index++);
      if (IsInterestingHash(digest)) {
         position = GetNumber(digest[5]);
         c = digest[6];
         return;
      }
   } while (true);
}

void DoPart2(const std::string& doorId) {
   char password[9] = "________";
   unsigned int index = 0; // Cheating
   std::cout << "Door code: " << password << std::endl;

   do {
      char c;
      size_t p;
      FindInterestingCharacter_Part2(doorId, index, c, p);
      if (0 <= p && p < 8 && password[p] == '_') {
         password[p] = c;
         std::cout << "Door code: " << password << std::endl;
      }
   } while (std::end(password) != std::find(std::begin(password), std::end(password), '_'));
}

int main(int argc, char *argv[]) {
   if (argc != 3) {
      std::cerr << "I need the door code and which part to do." << std::endl;
      return -1;
   }

   std::string doorId{ argv[1] };
   int part{ atoi(argv[2]) };

   switch (part) {
   case 1:
      DoPart1(doorId);
      break;
   case 2:
      DoPart2(doorId);
      break;
   default:
      std::cerr << "Don't know what part to do: " << part << std::endl;
      return -1;
   }

   return 0;
}
