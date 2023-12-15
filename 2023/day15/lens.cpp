#include "lens.h"

#include <sstream>
#include <string>

Lens::Lens(const std::string& label, const unsigned int focal_length)
  : m_label{ label },
    m_focal_length{ focal_length }
{ }

std::string Lens::label() const
{
  return m_label;
}

unsigned int Lens::focal_length() const
{
  return m_focal_length;
}

bool Lens::operator==(const Lens& other)
{
  return m_label.compare(other.m_label) == 0;
}

std::string Lens::to_string() const
{
  std::stringstream str;
  str << "[" << m_label << " " << m_focal_length << "]";
  return str.str();
}
