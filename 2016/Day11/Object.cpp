#include "stdafx.h"
#include "Object.h"

Object::Object(const ObjectType type, const std::wstring& material) : m_type{ type }, m_material{ material } {}

ObjectType Object::GetType() const {
   return m_type;
}

std::wstring Object::GetMaterial() const {
   return m_material;
}

bool Object::operator==(const Object& rhs) const {
   return (m_type == rhs.GetType() && m_material == rhs.GetMaterial());
}
