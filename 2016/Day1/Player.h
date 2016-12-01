#pragma once

#include "Instruction.h"
#include "Orientation.h"
#include "Position.h"

class Player {
public:
   Player();
   bool FollowInstruction(const Instruction& instruction);

   Position getPosition() const;

private:
   Orientation m_direction;
   Position m_position;
};
