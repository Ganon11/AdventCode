// Day3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Triangle.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<Triangle> GetPart1Triangles(const std::wstring& filename) {
   std::vector<Triangle> triangles;
   std::wifstream input{ filename };
   if (!input.is_open()) {
      return triangles;
   }

   do {
      int a, b, c;
      input >> a >> b >> c;
      triangles.push_back(Triangle(a, b, c));
   } while (input.good());

   return triangles;
}

std::vector<Triangle> GetPart2Triangles(const std::wstring& filename) {
   std::vector<Triangle> triangles;
   std::wifstream input{ filename };
   if (!input.is_open()) {
      return triangles;
   }

   std::vector<std::vector<int>> values;
   do {
      int a, b, c;
      input >> a >> b >> c;
      values.push_back(std::vector<int>{ a, b, c });
   } while (input.good());

   size_t numRows = values.size() / 3;

   for (size_t row = 0; row < numRows; ++row) {
      for (size_t col = 0; col < 3; ++col) {
         triangles.push_back(Triangle(values[(3 * row)][col], values[(3 * row) + 1][col], values[(3 * row) + 2][col]));
      }
   }

   return triangles;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Sample.txt";
   }

   auto part1Triangles = GetPart1Triangles(inputFile);
   auto part1 = std::count_if(std::begin(part1Triangles), std::end(part1Triangles), [](const Triangle& t) { return t.IsValid(); });
   std::wcout << L"Part 1: " << part1 << std::endl;

   auto part2Triangles = GetPart2Triangles(inputFile);
   auto part2 = std::count_if(std::begin(part2Triangles), std::end(part2Triangles), [](const Triangle& t) { return t.IsValid(); });
   std::wcout << L"Part 2: " << part2 << std::endl;

   return 0;
}
