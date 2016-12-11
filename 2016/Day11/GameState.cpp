#include "stdafx.h"
#include "GameState.h"

GameState::GameState(const unsigned int floor, const FloorState& floorState, const unsigned int moves)
   : m_currentFloor{ floor }, m_floorState{ floorState }, m_moves{ moves }
{}

unsigned int GameState::GetCurrentFloor() const {
   return m_currentFloor;
}

FloorState GameState::GetFloorState() const {
   return m_floorState;
}

unsigned int GameState::GetMoveCount() const {
   return m_moves;
}

bool GameState::IsFinalState() const {
   return (0 == m_floorState[0].GetObjectCount() && 0 == m_floorState[1].GetObjectCount() && 0 == m_floorState[2].GetObjectCount());
}

bool GameState::operator==(const GameState& rhs) const {
   unsigned int floor{ rhs.GetCurrentFloor() };
   if (m_currentFloor != floor) {
      return false;
   }

   FloorState otherFloorState{ rhs.GetFloorState() };
   return std::equal(std::begin(m_floorState), std::end(m_floorState), std::begin(otherFloorState));
}

namespace {
   std::vector<std::vector<Object>> Combinations(const std::vector<Object>& list, const unsigned int n) {
      std::vector<std::vector<Object>> combinations;
      if (n > list.size()) {
         return combinations;
      }

      if (n == list.size()) {
         combinations.push_back(list);
         return combinations;
      }

      if (1 == n) {
         for (const Object& o : list) {
            std::vector<Object> c;
            c.push_back(o);
            combinations.push_back(c);
         }
         return combinations;
      }

      for (size_t i = 0; i + n <= list.size(); ++i) {
         Object o{ list[i] };
         std::vector<Object> remainingObjects{ std::find(std::begin(list), std::end(list), o) + 1, std::end(list) };
         std::vector<std::vector<Object>> e = Combinations(remainingObjects, n - 1);
         for (std::vector<Object>& c : e) {
            c.push_back(o);
         }
         combinations.insert(std::end(combinations), std::begin(e), std::end(e));
      }

      return combinations;
   }
}

std::vector<GameState> GameState::GetMoves() const {
   std::vector<GameState> moves;

   std::vector<unsigned int> newFloors;
   if (m_currentFloor == 0) {
      newFloors.push_back(1);
   }
   else if (m_currentFloor == 3) {
      newFloors.push_back(2);
   }
   else {
      newFloors.push_back(m_currentFloor - 1);
      newFloors.push_back(m_currentFloor + 1);
   }

   std::vector<Object> currentFloorObjects{ m_floorState[m_currentFloor].GetAllObjects() };
   std::vector<std::vector<Object>> objectCombinations = Combinations(currentFloorObjects, 1);
   std::vector<std::vector<Object>> twoObjectCombinations = Combinations(currentFloorObjects, 2);
   objectCombinations.insert(std::end(objectCombinations), std::begin(twoObjectCombinations), std::end(twoObjectCombinations));

   for (const std::vector<Object>& objectsToRemove : objectCombinations) {
      if (std::end(objectsToRemove) == std::find_if(std::begin(objectsToRemove), std::end(objectsToRemove), [](const Object& o) { return o.GetType() == OBJECT_CHIP; })) {
         // Can't travel without a chip.
         continue;
      }

      Floor newCurrentFloor{ m_floorState[m_currentFloor].RemoveObjects(objectsToRemove) };
      if (!newCurrentFloor.IsValid()) {
         continue;
      }

      for (const unsigned int newFloor : newFloors) {
         Floor newFloorContents{ m_floorState[newFloor].AddObjects(objectsToRemove) };

         if (!newFloorContents.IsValid()) {
            continue;
         }

         FloorState modifiedFloorState{ m_floorState };
         modifiedFloorState[m_currentFloor] = newCurrentFloor;
         modifiedFloorState[newFloor] = newFloorContents;

         moves.push_back(GameState{ newFloor, modifiedFloorState, m_moves + 1 });
      }
   }

   return moves;
}
