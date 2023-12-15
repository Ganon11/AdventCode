#pragma once

#include "functional"
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "lens.h"

enum HASHMAPOperation
{
  REMOVAL,
  INSERTION
};

unsigned int hash(const std::string& text);

class HASHMAP
{
public:
  HASHMAP(const std::function<unsigned char(const std::string&)> hash_func = hash);

  void process_command(const std::string& command);
  void insert(const unsigned int index, const Lens& lens);
  void remove(const unsigned int index, const std::string& label);

  unsigned long long focusing_power() const;

  std::string to_string() const;

private:
  std::map<unsigned int, std::vector<Lens>> m_boxes;
  std::function<unsigned char(const std::string&)> m_hash;
};
