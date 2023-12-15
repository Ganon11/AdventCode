#pragma once

#include "functional"
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "lens.h"


unsigned int hash(const std::string& text);

class HASHMAP
{
public:
  typedef std::function<unsigned char(const std::string&)> HashFunc;

  HASHMAP(const HashFunc& hash_func = hash);

  void process_command(const std::string& command);

  unsigned long long focusing_power() const;
  std::string to_string() const;

private:
  void insert(const unsigned int index, const Lens& lens);
  void remove(const unsigned int index, const std::string& label);

  typedef std::vector<Lens> Box;
  typedef std::map<unsigned char, Box> Boxes;
  Boxes m_boxes;
  HashFunc m_hash;
};
