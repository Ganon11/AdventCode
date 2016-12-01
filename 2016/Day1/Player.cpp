#include "stdafx.h"
#include "Player.h"

Player::Player() : m_direction{ NORTH }, m_position{ 0, 0 } {}

bool Player::FollowInstruction(const Instruction& instruction) {
   m_direction = Rotate(m_direction, instruction.getRotation());
   m_position = Position::Move(m_position, instruction.getBlocks(), m_direction);
   return true;
}

Position Player::getPosition() const {
   return m_position;
}
