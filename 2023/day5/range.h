#pragma once

class Range
{
public:
  Range() = delete;
  Range(unsigned long long start, unsigned long long length);

  unsigned long long start() const { return m_start; }
  unsigned long long length() const { return m_length; }

private:
  unsigned long long m_start;
  unsigned long long m_length;
};
