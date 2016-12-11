#pragma once

#include "Object.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

class Floor {
public:
   Floor(const std::wstring& line);
   Floor(const unsigned int num, const std::vector<Object>& objects);

   unsigned int GetNum() const;
   std::vector<Object> GetChips() const;
   std::vector<Object> GetGenerators() const;
   std::vector<Object> GetAllObjects() const;

   unsigned int GetObjectCount() const;

   bool IsValid() const;
   Floor AddObjects(const std::vector<Object>& newObjects) const;
   Floor RemoveObjects(const std::vector<Object>& objectsToRemove) const;

   bool operator==(const Floor& rhs) const;

private:
   unsigned int m_num;
   std::vector<Object> m_chips;
   std::vector<Object> m_generators;
};

typedef std::vector<Floor> FloorState;

namespace std {
   template<> struct hash<Floor> {
      size_t operator()(const Floor& f) const noexcept {
         std::wstringstream str;
         str << L"floor " << f.GetNum() << L" ";
         std::vector<Object> objects{ f.GetAllObjects() };
         std::sort(std::begin(objects), std::end(objects));
         for (const Object& o : objects) {
            str << o.GetType() << o.GetMaterial() << L" ";
         }
         return std::hash<std::wstring>()(str.str());
      }
   };

   template<> struct hash<FloorState> {
      size_t operator()(const FloorState& fs) const noexcept {
         size_t h{ 1 };
         for (const Floor& f : fs) {
            h *= std::hash<Floor>()(f);
         }
         return h;
      }
   };
}
