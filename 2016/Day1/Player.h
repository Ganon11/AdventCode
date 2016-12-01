#pragma once

#include "Instruction.h"
#include "Orientation.h"
#include "Position.h"

#include <set>

class Player {
public:
   Player();
   bool FollowInstruction(const Instruction& instruction);

   Position getPosition() const;

   bool foundIntersection() const;
   Position getIntersection() const;

private:
   Orientation m_direction;
   Position m_position;

   std::set<Position> m_previousPositions;
   Position m_intersection;
   bool m_foundIntersection;
};
