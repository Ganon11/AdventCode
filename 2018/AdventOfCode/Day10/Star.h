#pragma once

#include <string>
#include "../AoCHelpers/Position.h"

typedef std::pair<int, int> Velocity;

class Star
{
public:
  Star(const std::wstring& line);

  void move();
  advent_of_code::Position get_position() const;

private:
  advent_of_code::Position m_position;
  Velocity m_velocity;
};

