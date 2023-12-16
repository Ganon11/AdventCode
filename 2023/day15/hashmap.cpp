#include "hashmap.h"

#include <algorithm>
#include <iostream>
#include <sstream>

HASHMAP::HASHMAP(const HashFunc& hash)
  : m_hash{ hash }
{ }

void HASHMAP::process_command(const std::string& command)
{
  size_t position{ command.find('=') };
  std::string label;
  unsigned focal_length;
  if (std::string::npos == position)
  {
    label = command.substr(0, command.size() - 1);
    remove(m_hash(label), label);
  }
  else
  {
    label = command.substr(0, position);
    focal_length = command[position + 1] - '0';
    insert(m_hash(label), Lens{ label, focal_length });
  }
}

HASHMAP::Lens::Lens(const std::string& label, const unsigned focal_length)
  : label{ label },
    focal_length{ focal_length }
{ }

void HASHMAP::insert(const unsigned char index, const Lens& lens)
{
  if (m_boxes.end() == m_boxes.find(index))
  {
    m_boxes.insert(Boxes::value_type(index, Box{}));
  }

  std::vector<Lens>& box{ m_boxes.at(index) };
  auto position{ std::find_if(box.begin(), box.end(), [&lens](const Lens& l){ return lens.label.compare(l.label) == 0; }) };
  if (box.end() == position)
  {
    box.push_back(lens);
  }
  else
  {
    position->focal_length = lens.focal_length;
  }
}

void HASHMAP::remove(const unsigned char index, const std::string& label)
{
  if (m_boxes.end() == m_boxes.find(index))
  {
    return;
  }

  std::vector<Lens>& box{ m_boxes.at(index) };
  auto position{ std::find_if(box.begin(), box.end(), [&label](const Lens& l){ return label.compare(l.label) == 0; }) };
  if (box.end() == position)
  {
    return;
  }

  box.erase(position);
}

unsigned long long HASHMAP::focusing_power() const
{
  unsigned long long sum{0};

  for (const auto& kvp : m_boxes)
  {
    unsigned long long box_factor{ 1ull + kvp.first };
    for (size_t lens_index = 0; lens_index < kvp.second.size(); ++lens_index)
    {
      unsigned long long slot_factor{ 1ull + lens_index };
      sum += box_factor * slot_factor * kvp.second[lens_index].focal_length;
    }
  }

  return sum;
}

unsigned char hash(const std::string& text)
{
  unsigned int current{0};
  for (const char ch : text)
  {
    current += static_cast<unsigned int>(ch);
    current *= 17;
    current %= 256;
  }

  return static_cast<unsigned char>(current);
}
