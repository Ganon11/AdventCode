#include "stdafx.h"
#include "Floor.h"
#include <algorithm>
#include <iostream>
#include <regex>

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

Floor::Floor(const std::wstring& line) {
   std::wregex floorNumRe{ L"The (\\w+) floor contains" };
   std::wsmatch floorNumMatch;
   if (!std::regex_search(line, floorNumMatch, floorNumRe)) {
      std::wcerr << L"Invalid line." << std::endl;
      throw L"Invalid line.";
   }
   m_num = GetFloorNum(floorNumMatch[1]);

   std::wregex chipRe{ L"a (\\w+)-compatible microchip" };
   std::wsmatch chipMatch;
   std::wstring::const_iterator search{ std::begin(line) }, end{ std::end(line) };
   while (std::regex_search(search, end, chipMatch, chipRe)) {
      std::wstring material{ chipMatch[1] };
      m_chips.push_back(Object{ OBJECT_CHIP, material });
      search += chipMatch.position() + chipMatch.length();
   }

   std::wregex genRe{ L"a (\\w+) generator" };
   std::wsmatch genMatch;
   search = std::begin(line);
   while (std::regex_search(search, end, genMatch, genRe)) {
      std::wstring material{ genMatch[1] };
      m_generators.push_back(Object{ OBJECT_GENERATOR, material });
      search += genMatch.position() + genMatch.length();
   }
}

Floor::Floor(const unsigned int num, const std::vector<Object>& objects) : m_num{ num } {
   for (const Object& o : objects) {
      switch (o.GetType()) {
      case OBJECT_CHIP:
         m_chips.push_back(o);
         break;
      case OBJECT_GENERATOR:
         m_generators.push_back(o);
         break;
      }
   }
}

unsigned int Floor::GetNum() const {
   return m_num;
}

std::vector<Object> Floor::GetChips() const {
   return m_chips;
}

std::vector<Object> Floor::GetGenerators() const {
   return m_generators;
}

std::vector<Object> Floor::GetAllObjects() const {
   std::vector<Object> objects;
   objects.insert(std::end(objects), std::begin(m_chips), std::end(m_chips));
   objects.insert(std::end(objects), std::begin(m_generators), std::end(m_generators));
   return objects;
}

unsigned int Floor::GetObjectCount() const {
   return static_cast<unsigned int>(m_chips.size() + m_generators.size());
}

bool Floor::IsValid() const {
   if (m_generators.size() == 0) {
      return true;
   }
   else if (m_chips.size() == 0) {
      return true;
   }
   else {
      for (const Object& chip : m_chips) {
         std::wstring material{ chip.GetMaterial() };
         if (std::end(m_generators) == std::find_if(std::begin(m_generators),
            std::end(m_generators),
            [&material](const Object& g) { return material == g.GetMaterial(); })) {

            return false;
         }
      }
   }

   return true;
}

Floor Floor::AddObjects(const std::vector<Object>& newObjects) const {
   std::vector<Object> objects;
   objects.insert(std::end(objects), std::begin(m_chips), std::end(m_chips));
   objects.insert(std::end(objects), std::begin(m_generators), std::end(m_generators));
   objects.insert(std::end(objects), std::begin(newObjects), std::end(newObjects));
   return Floor{ m_num, objects };
}

Floor Floor::RemoveObjects(const std::vector<Object>& objectsToRemove) const {
   std::vector<Object> newChips{ m_chips }, newGenerators{ m_generators };
   for (const Object& o : objectsToRemove) {
      switch (o.GetType()) {
      case OBJECT_CHIP:
         newChips.erase(std::remove(std::begin(newChips), std::end(newChips), o), std::end(newChips));
         break;
      case OBJECT_GENERATOR:
         newGenerators.erase(std::remove(std::begin(newGenerators), std::end(newGenerators), o), std::end(newGenerators));
         break;
      }
   }

   std::vector<Object> newObjects;
   newObjects.insert(std::end(newObjects), std::begin(newChips), std::end(newChips));
   newObjects.insert(std::end(newObjects), std::begin(newGenerators), std::end(newGenerators));

   return Floor{ m_num, newObjects };
}

bool Floor::operator==(const Floor& rhs) const {
   unsigned int num{ rhs.GetNum() };
   if (m_num != num) {
      return false;
   }

   std::vector<Object> chips{ rhs.GetChips() };
   if (m_chips.size() != chips.size() || !std::is_permutation(std::begin(m_chips), std::end(m_chips), std::begin(chips))) {
      return false;
   }

   std::vector<Object> generators{ rhs.GetGenerators() };
   if (m_generators.size() != generators.size() || !std::is_permutation(std::begin(m_generators), std::end(m_generators), std::begin(generators))) {
      return false;
   }

   return true;
}
