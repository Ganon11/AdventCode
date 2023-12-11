#pragma once

#include <map>
#include <set>

#include "position.h"

using advent_of_code::Position;

enum PipeType
{
  NONE,
  GROUND,
  START,
  VERTICAL,
  HORIZONTAL,
  NORTH_EAST_BEND,
  NORTH_WEST_BEND,
  SOUTH_WEST_BEND,
  SOUTH_EAST_BEND
};

class MazeNode
{
public:
  MazeNode() = default;
  MazeNode(const Position& p);

  Position position() const;
  void set_type(const PipeType type);
  bool add_adjacent_position(const Position& p);
  bool can_reach(const Position& p) const;
  std::set<Position> exits() const;
  PipeType type() const;
  void deduce_type();

private:
  Position m_position;
  std::set<Position> m_exits;
  PipeType m_type;
};

typedef std::map<Position, MazeNode> Maze;
