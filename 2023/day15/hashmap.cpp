#include "hashmap.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "command.h"

void HASHMAP::process_command(const HASHMAPCommand& command)
{
  switch (command.operation())
  {
  case INSERTION:
    insert(command.target(), Lens{ command.label(), command.focal_length() });
    break;
  case REMOVAL:
    remove(command.target(), command.label());
    break;
  }

  // std::cout << "After \"" << command.to_string() << "\":" << std::endl;
  // std::cout << to_string() << std::endl;
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
