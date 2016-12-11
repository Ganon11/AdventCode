#pragma once

#include <string>

enum ObjectType {
   OBJECT_CHIP = 0,
   OBJECT_GENERATOR = 1
};

class Object {
public:
   Object(const ObjectType type, const std::wstring& material);

   ObjectType GetType() const;
   std::wstring GetMaterial() const;

   bool operator==(const Object& rhs) const;
   friend bool operator<(const Object& l, const Object& r) {
      if (l.m_type < r.m_type) {
         return true;
      }
      else if (l.m_type == r.m_type && l.m_material < r.m_material) {
         return true;
      }
      return false;
   }

private:
   ObjectType m_type;
   std::wstring m_material;
};
