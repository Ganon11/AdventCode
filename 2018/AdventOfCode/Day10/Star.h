#pragma once

#include <string>

typedef std::pair<int, int> Position;
typedef std::pair<int, int> Velocity;

class Star
{
public:
  Star(const std::wstring& line);

  void move();
  Position get_position() const;

private:
  Position m_position;
  Velocity m_velocity;
};

