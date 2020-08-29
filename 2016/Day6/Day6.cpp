// Day6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CharacterFrequency.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

CharacterCounts ReadInput(const std::wstring& inputFile) {
   CharacterCounts counts;
   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return counts;
   }

   do {
      std::wstring line;
      input >> line;
      for (size_t i = 0; i < line.length(); ++i) {
         wchar_t c{ line[i] };
         if (counts.size() == i) {
            counts.push_back(FrequencyHeap());
         }
         auto result = std::find(std::begin(counts[i]), std::end(counts[i]), c);
         if (std::end(counts[i]) == result) {
            counts[i].push_back(CharacterFrequency(c));
         }
         else {
            result->frequency++;
         }
      }
   } while (input.good());

   return counts;
}

std::wstring RestoreString(CharacterCounts& counts, const std::function<bool(const CharacterFrequency&, const CharacterFrequency&)> comp) {
   std::wstring string;

   for (FrequencyHeap heap : counts) {
      std::make_heap(std::begin(heap), std::end(heap), comp);
      string.push_back(heap[0].character);
   }

   return string;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"sample.txt";
   }

   auto counts = ReadInput(inputFile);

   auto maxHeapComparator = [](const CharacterFrequency& a, const CharacterFrequency& b) { return a.frequency < b.frequency; };
   std::wstring result = RestoreString(counts, maxHeapComparator);

   std::wcout << L"Result for part 1: " << result << std::endl;

   auto minHeapComparator = [](const CharacterFrequency& a, const CharacterFrequency& b) { return a.frequency > b.frequency; };
   result = RestoreString(counts, minHeapComparator);

   std::wcout << L"Result for part 2: " << result << std::endl;

   return 0;
}
