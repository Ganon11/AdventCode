#pragma once

#include "Floor.h"
#include <vector>

class GameState {
public:
   GameState(const unsigned int floor, const FloorState& floorState, const unsigned int moves);

   unsigned int GetCurrentFloor() const;
   FloorState GetFloorState() const;
   unsigned int GetMoveCount() const;
   bool IsFinalState() const;

   bool operator==(const GameState& rhs) const;

   std::vector<GameState> GetMoves() const;

private:
   unsigned int m_currentFloor;
   FloorState m_floorState;
   unsigned int m_moves;
};

namespace std {
   template<> struct hash<GameState> {
      size_t operator()(const GameState& gs) const noexcept {
         return (gs.GetCurrentFloor() + 1) * std::hash<FloorState>()(gs.GetFloorState());
      }
   };
}
