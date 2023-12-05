#pragma once

#include <string>

class ConversionRange
{
public:
  ConversionRange() = delete;
  ConversionRange(const std::string& line);

  bool contains(const unsigned long long source) const;
  unsigned long long convert(const unsigned long long source) const;

  unsigned long long destination() const { return m_destination_start; }

private:
  unsigned long long m_destination_start{0};
  unsigned long long m_source_start{0};
  unsigned long long m_length{0};
};
