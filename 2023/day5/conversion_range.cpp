#include "conversion_range.h"

#include <vector>

#include "input_handler.h"

ConversionRange::ConversionRange(const std::string& line)
{
  std::vector<std::string> tokens = advent_of_code::tokenize(line, ' ');
  m_destination_start = std::stoull(tokens[0]);
  m_source_start = std::stoull(tokens[1]);
  m_length = std::stoull(tokens[2]);
}

bool ConversionRange::contains(const unsigned long long source) const
{
  return source >= m_source_start && (source - m_source_start) < m_length;
}

unsigned long long ConversionRange::convert(const unsigned long long source) const
{
  return m_destination_start + (source - m_source_start);
}
