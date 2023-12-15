#include "hashmap.h"

#include <algorithm>
#include <iostream>
#include <sstream>

HASHMAP::HASHMAP(const std::function<unsigned char(const std::string&)> hash)
  : m_hash{ hash }
{ }

void HASHMAP::process_command(const std::string& command)
{
  size_t position{ command.find('=') };
  HASHMAPOperation operation;
  std::string label;
  unsigned focal_length;
  if (std::string::npos == position)
  {
    operation = REMOVAL;
    label = command.substr(0, command.size() - 1);
    remove(m_hash(label), label);
  }
  else
  {
    operation = INSERTION;
    label = command.substr(0, position);
    focal_length = command[position + 1] - '0';
    insert(m_hash(label), Lens{ label, focal_length });
  }
}

void HASHMAP::insert(const unsigned int index, const Lens& lens)
{
  if (m_boxes.end() == m_boxes.find(index))
  {
    m_boxes.insert(std::map<unsigned int, std::vector<Lens>>::value_type(index, std::vector<Lens>{}));
  }

  std::vector<Lens>& box{ m_boxes.at(index) };
  auto position{ std::find(box.begin(), box.end(), lens) };
  if (box.end() == position)
  {
    box.push_back(lens);
  }
  else
  {
    *position = lens;
  }
}

void HASHMAP::remove(const unsigned int index, const std::string& label)
{
  if (m_boxes.end() == m_boxes.find(index))
  {
    return;
  }

  std::vector<Lens>& box{ m_boxes.at(index) };
  auto position{ std::find_if(box.begin(), box.end(), [&label](const Lens& l){ return label.compare(l.label()) == 0; }) };
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
      sum += box_factor * slot_factor * kvp.second[lens_index].focal_length();
    }
  }

  return sum;
}

std::string HASHMAP::to_string() const
{
  std::stringstream str;
  for (const auto& kvp : m_boxes)
  {
    if (kvp.second.empty())
    {
      continue;
    }

    str << "Box " << kvp.first << ": ";
    for (const Lens& lens : kvp.second)
    {
      str << lens.to_string() << " ";
    }

    str << std::endl;
  }

  return str.str();
}
