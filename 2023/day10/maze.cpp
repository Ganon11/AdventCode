#include "maze.h"
#include "position.h"

using advent_of_code::Position;

MazeNode::MazeNode(const Position& p) : m_position{ p }
{ }

Position MazeNode::position() const
{
  return m_position;
}

void MazeNode::set_type(const PipeType type)
{
  m_type = type;
  switch (m_type)
  {
    case GROUND:
    case START:
    case NONE:
      break;
    case VERTICAL:
      m_exits.insert(m_position.north());
      m_exits.insert(m_position.south());
      break;
    case HORIZONTAL:
      m_exits.insert(m_position.east());
      m_exits.insert(m_position.west());
      break;
    case NORTH_EAST_BEND:
      m_exits.insert(m_position.north());
      m_exits.insert(m_position.east());
      break;
    case NORTH_WEST_BEND:
      m_exits.insert(m_position.north());
      m_exits.insert(m_position.west());
      break;
    case SOUTH_WEST_BEND:
      m_exits.insert(m_position.south());
      m_exits.insert(m_position.west());
      break;
    case SOUTH_EAST_BEND:
      m_exits.insert(m_position.south());
      m_exits.insert(m_position.east());
      break;
  }
}

bool MazeNode::add_adjacent_position(const Position& p)
{
  if (m_exits.size() >= 2)
  {
    return false;
  }

  if (std::find(m_exits.begin(), m_exits.end(), p) != m_exits.end())
  {
    return false;
  }

  m_exits.insert(p);
  return true;
}

bool MazeNode::can_reach(const Position& p) const
{
  return std::find(m_exits.begin(), m_exits.end(), p) != m_exits.end();
}

std::set<Position> MazeNode::exits() const
{
  return m_exits;
}

PipeType MazeNode::type() const
{
  return m_type;
}

void MazeNode::deduce_type()
{
  if (m_exits.size() != 2)
  {
    std::cerr << "Cannot deduce type!" << std::endl;
    return;
  }

  Position n{ m_position.north() };
  Position s{ m_position.south() };
  Position e{ m_position.east() };
  Position w{ m_position.west() };

  if (m_exits.find(n) != m_exits.end() && m_exits.find(s) != m_exits.end())
  {
    m_type = VERTICAL;
  }
  else if (m_exits.find(e) != m_exits.end() && m_exits.find(w) != m_exits.end())
  {
    m_type = HORIZONTAL;
  }
  else if (m_exits.find(n) != m_exits.end() && m_exits.find(e) != m_exits.end())
  {
    m_type = NORTH_EAST_BEND;
  }
  else if (m_exits.find(n) != m_exits.end() && m_exits.find(w) != m_exits.end())
  {
    m_type = NORTH_WEST_BEND;
  }
  else if (m_exits.find(s) != m_exits.end() && m_exits.find(e) != m_exits.end())
  {
    m_type = SOUTH_EAST_BEND;
  }
  else if (m_exits.find(s) != m_exits.end() && m_exits.find(w) != m_exits.end())
  {
    m_type = SOUTH_WEST_BEND;
  }
}
