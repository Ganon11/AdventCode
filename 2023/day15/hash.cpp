#include "hash.h"

#include <string>

unsigned int hash(const std::string& text)
{
  unsigned int current{0};
  for (const char ch : text)
  {
    current += static_cast<unsigned int>(ch);
    current *= 17;
    current %= 256;
  }

  return current;
}