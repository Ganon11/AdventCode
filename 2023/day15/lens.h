#pragma once

#include <string>

class Lens
{
public:
  Lens(const std::string& label, const unsigned int focal_length);

  std::string label() const;
  unsigned int focal_length() const;

  bool operator==(const Lens& other);

  std::string to_string() const;

private:
  std::string m_label;
  unsigned int m_focal_length;
};
