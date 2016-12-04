#include "stdafx.h"
#include "Room.h"

#include <algorithm>
#include <map>
#include <regex>

Room::Room(const std::wstring& input) {
   std::wregex re{ L"([a-z\\-]+)\\-(\\d+)\\[([a-z]+)\\]" };
   std::wsmatch m;

   if (std::regex_match(input, m, re)) {
      m_name = m[1].str();
      m_sectorID = _wtoi(m[2].str().c_str());

      m_checksum.reserve(5);
      for (const wchar_t c : m[3].str()) {
         m_checksum.push_back(c);
      }
   }
}

bool Room::IsValid() const {
   std::map<wchar_t, int> counts;
   for (const wchar_t c : m_name) {
      if (c == L'-') {
         continue;
      }

      if (std::end(counts) == counts.find(c)) {
         counts.insert(std::make_pair(c, 1));
      }
      else {
         ++counts[c];
      }
   }

   std::vector<std::pair<wchar_t, int>> pairs;
   for (const auto& pair : counts) {
      pairs.push_back(pair);
   }

   sort(pairs.begin(), pairs.end(), [](const std::pair<wchar_t, int>& a, const std::pair<wchar_t, int>& b) {
      if (b.second < a.second) {
         return true;
      }
      else if (b.second == a.second && b.first > a.first) {
         return true;
      }

      return false;
   });

   for (int i = 0; i < 5; ++i) {
      if (m_checksum[i] != pairs[i].first) {
         return false;
      }
   }

   return true;
}

int Room::GetSectorId() const {
   return m_sectorID;
}

std::wstring Room::GetDecryptedName() const {
   std::wstring name;

   for (const wchar_t c : m_name) {
      if (c == L'-') {
         name.push_back(L' ');
      }
      else {
         name.push_back(((c - L'a' + m_sectorID) % 26) + L'a');
      }
   }

   return name;
}

std::wstring Room::GetEncryptedName() const {
   return m_name;
}
