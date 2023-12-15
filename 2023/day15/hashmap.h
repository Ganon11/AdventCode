#pragma once

#include <map>
#include <string>
#include <vector>

#include "command.h"
#include "hash.h"
#include "lens.h"

class HASHMAP
{
public:
  HASHMAP() = default;

  void process_command(const HASHMAPCommand& command);
  void insert(const unsigned int index, const Lens& lens);
  void remove(const unsigned int index, const std::string& label);

  unsigned long long focusing_power() const;

  std::string to_string() const;

private:
  std::map<unsigned int, std::vector<Lens>> m_boxes;
};
