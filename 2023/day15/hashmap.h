#pragma once

#include "functional"
#include <functional>
#include <map>
#include <string>
#include <vector>

unsigned char hash(const std::string& text);

class HASHMAP
{
public:
  typedef std::function<unsigned char(const std::string&)> HashFunc;

  HASHMAP(const HashFunc& hash_func = hash);

  void process_command(const std::string& command);

  unsigned long long focusing_power() const;

private:
  struct Lens
  {
    Lens(const std::string& label, const unsigned focal_length);

    std::string label;
    unsigned focal_length;
  };

  void insert(const unsigned char index, const Lens& lens);
  void remove(const unsigned char index, const std::string& label);

  typedef std::vector<Lens> Box;
  typedef std::map<unsigned char, Box> Boxes;
  Boxes m_boxes;
  HashFunc m_hash;
};
