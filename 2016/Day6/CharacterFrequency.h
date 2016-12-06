#pragma once

#include <vector>

struct CharacterFrequency {
   wchar_t character;
   int frequency;

   CharacterFrequency(const wchar_t c);

   bool operator==(const CharacterFrequency& rhs);
   bool operator==(const wchar_t c);
};

typedef std::vector<CharacterFrequency> FrequencyHeap;
typedef std::vector<FrequencyHeap> CharacterCounts;
