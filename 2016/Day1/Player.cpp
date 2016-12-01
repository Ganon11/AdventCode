#include "stdafx.h"
#include "Player.h"

Player::Player() : m_direction{ NORTH }, m_position{ 0, 0 }, m_foundIntersection{ false } {
   m_previousPositions.insert(m_position);
}

bool Player::FollowInstruction(const Instruction& instruction) {
   m_direction = Rotate(m_direction, instruction.getRotation());

   if (m_foundIntersection) {
      m_position = Position::Move(m_position, instruction.getBlocks(), m_direction);
   }
   else {
      const auto& intermediatePositions = Position::MoveAndReturnIntermediatePositions(m_position, instruction.getBlocks(), m_direction);
      for (const auto& position : intermediatePositions) {
         if (m_previousPositions.find(position) == std::end(m_previousPositions)) {
            m_previousPositions.insert(position);
         }
         else {
            m_intersection = position;
            m_foundIntersection = true;
            break;
         }
      }
      m_position = *intermediatePositions.rbegin();
   }
   return true;
}

Position Player::getPosition() const {
   return m_position;
}

bool Player::foundIntersection() const {
   return m_foundIntersection;
}

Position Player::getIntersection() const {
   return m_intersection;
}
