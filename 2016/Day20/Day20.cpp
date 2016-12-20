// Day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

std::vector<bool> GetIPs(const std::wstring& inputFile) {
   size_t size{ std::numeric_limits<unsigned int>::max() };
   std::vector<bool> ips(size, true);

   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return ips;
   }

   do {
      size_t start, end;
      wchar_t c;
      input >> start >> c >> end;
      for (size_t i = start; i <= end; ++i) {
         ips[i] = false;
      }
   } while (input.good());

   return ips;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Sample.txt";
   }

   auto ips{ GetIPs(inputFile) };

   auto count{ std::count_if(std::begin(ips), std::end(ips), [](const bool b) { return b; }) };

   std::wcout << count << std::endl;

   for (size_t i = 0; i < ips.size(); ++i) {
      if (ips[i]) {
         std::wcout << i << std::endl;
         break;
      }
   }

   return 0;
}
