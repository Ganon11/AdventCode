#include "stdafx.h"
#include "GameState.h"
#include <algorithm>

//GameState::GameState(const unsigned int floor, const FloorState& floorState, const unsigned int moves)
//   : m_currentFloor{ floor }, m_floorState{ floorState }, m_moves{ moves }
//{}

GameState::GameState(const unsigned int floor, const ObjectState& objectState, const unsigned int moves)
   : m_currentFloor{ floor }, m_objectState{ objectState }, m_moves{ moves }
{}

unsigned int GameState::GetCurrentFloor() const {
   return m_currentFloor;
}

//FloorState GameState::GetFloorState() const {
//   return m_floorState;
//}

ObjectState GameState::GetObjectState() const {
   return m_objectState;
}

unsigned int GameState::GetMoveCount() const {
   return m_moves;
}

unsigned int GameState::GetFloorObjectCount(const unsigned int floor) const {
   unsigned int count{ 0 };
   for (const ObjectPair& op : m_objectState) {
      if (floor == op.GetChipFloor()) {
         ++count;
      }
      if (floor == op.GetGeneratorFloor()) {
         ++count;
      }
   }
   return count;
}

bool GameState::IsFinalState() const {
   //return (0 == m_floorState[0].GetObjectCount() && 0 == m_floorState[1].GetObjectCount() && 0 == m_floorState[2].GetObjectCount());
   return (0 == GetFloorObjectCount(0) && 0 == GetFloorObjectCount(1) && 0 == GetFloorObjectCount(2));
}

//namespace {
//   unsigned int GetNumPairs(const Floor& floor) {
//      unsigned int pairs{ 0 };
//      std::vector<Object> chips{ floor.GetChips() };
//      std::vector<Object> generators{ floor.GetGenerators() };
//      for (const Object& c : chips) {
//         std::wstring material{ c.GetMaterial() };
//         if (std::end(generators) != std::find_if(std::begin(generators), std::end(generators), [&material](const Object& g) { return material == g.GetMaterial(); })) {
//            ++pairs;
//         }
//      }
//
//      return pairs;
//   }
//
//   std::vector<Object> GetUnpairedObjects(const Floor& floor) {
//      std::vector<Object> unpaired, objects{ floor.GetAllObjects() };
//      for (const Object& o : objects) {
//         ObjectType pairType{ o.GetType() == OBJECT_CHIP ? OBJECT_GENERATOR : OBJECT_CHIP };
//         Object pairObject{ pairType, o.GetMaterial() };
//         if (std::end(objects) == std::find(std::begin(objects), std::end(objects), pairObject)) {
//            unpaired.push_back(o);
//         }
//      }
//      return unpaired;
//   }
//}

bool GameState::operator==(const GameState& rhs) const {
   unsigned int floor{ rhs.GetCurrentFloor() };
   if (m_currentFloor != floor) {
      return false;
   }

   ObjectState otherObjectState{ rhs.GetObjectState() };
   if (m_objectState.size() != otherObjectState.size()) {
      return false;
   }

   for (const ObjectPair& ourPair : m_objectState) {
      if (std::end(otherObjectState) == std::find(std::begin(otherObjectState), std::end(otherObjectState), ourPair)) {
         return false;
      }
   }

   return true;

   //ObjectState ourObjectState{ m_objectState };
   //std::sort(std::begin(ourObjectState), std::end(ourObjectState));
   //std::sort(std::begin(otherObjectState), std::end(otherObjectState));
   //return std::equal(std::begin(ourObjectState), std::end(ourObjectState), std::begin(otherObjectState));

   //FloorState otherFloorState{ rhs.GetFloorState() };
   //for (size_t i = 0; i < 4; ++i) {
   //   if (GetNumPairs(m_floorState[i]) != GetNumPairs(otherFloorState[i])) {
   //      return false;
   //   }

   //   std::vector<Object> ourUnpaired{ GetUnpairedObjects(m_floorState[i]) }, theirUnpaired{ GetUnpairedObjects(otherFloorState[i]) };
   //   if (ourUnpaired.size() != theirUnpaired.size()) {
   //      return false;
   //   }

   //   std::sort(std::begin(ourUnpaired), std::end(ourUnpaired));
   //   std::sort(std::begin(theirUnpaired), std::end(theirUnpaired));
   //   if (!std::equal(std::begin(ourUnpaired), std::end(ourUnpaired), std::begin(theirUnpaired))) {
   //      return false;
   //   }
   //}

   //return true;

   //return std::equal(std::begin(m_floorState), std::end(m_floorState), std::begin(otherFloorState));
}

namespace {
//   std::vector<std::vector<Object>> Combinations(const std::vector<Object>& list, const unsigned int n) {
//      std::vector<std::vector<Object>> combinations;
//      if (n > list.size()) {
//         return combinations;
//      }
//
//      if (n == list.size()) {
//         combinations.push_back(list);
//         return combinations;
//      }
//
//      if (1 == n) {
//         for (const Object& o : list) {
//            std::vector<Object> c;
//            c.push_back(o);
//            combinations.push_back(c);
//         }
//         return combinations;
//      }
//
//      for (size_t i = 0; i + n <= list.size(); ++i) {
//         Object o{ list[i] };
//         std::vector<Object> remainingObjects{ std::find(std::begin(list), std::end(list), o) + 1, std::end(list) };
//         std::vector<std::vector<Object>> e = Combinations(remainingObjects, n - 1);
//         for (std::vector<Object>& c : e) {
//            c.push_back(o);
//         }
//         combinations.insert(std::end(combinations), std::begin(e), std::end(e));
//      }
//
//      return combinations;
//   }

   bool ObjectStateIsValid(const ObjectState& state) {
      for (const ObjectPair& pair : state) {
         unsigned int chipFloor{ pair.GetChipFloor() };

         // A chip is safe if connected to its generator.
         if (chipFloor == pair.GetGeneratorFloor()) {
            continue;
         }

         // A chip is safe if no other generators are on the floor.
         ObjectState otherObjects{ state };
         otherObjects.erase(std::remove(std::begin(otherObjects), std::end(otherObjects), pair));
         for (const ObjectPair& otherPair : otherObjects) {
            if (chipFloor == otherPair.GetGeneratorFloor()) {
               return false;
            }
         }
      }

      return true;
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
      // If all lower floors are empty, don't bother going down a floor.
      bool floorsEmpty{ true };
      for (unsigned int i = 0; i < m_currentFloor; ++i) {
         if (static_cast<unsigned int>(0) != GetFloorObjectCount(i)) {
            floorsEmpty = false;
            break;
         }
         /*if (0 != m_floorState[i].GetObjectCount()) {
            floorsEmpty = false;
            break;
         }*/
      }

      if (!floorsEmpty) {
         newFloors.push_back(m_currentFloor - 1);
      }

      newFloors.push_back(m_currentFloor + 1);
   }

   //unsigned int currentFloorObjectCount{ GetFloorObjectCount(m_currentFloor) };

   for (const unsigned int newFloor : newFloors) {
      // First, consider moving one object, or a pair of objects
      for (size_t i = 0; i < m_objectState.size(); ++i) {
         bool chipOnFloor{ m_currentFloor == m_objectState[i].GetChipFloor() };
         bool generatorOnFloor{ m_currentFloor == m_objectState[i].GetGeneratorFloor() };

         if (!chipOnFloor && !generatorOnFloor) {
            // Can't move either of these
            continue;
         }

         if (chipOnFloor && generatorOnFloor) {
            // We can move both the chip and the generator. Guaranteed to be a safe object state
            ObjectState modifiedObjectState{ m_objectState };
            modifiedObjectState[i].SetChipFloor(newFloor);
            modifiedObjectState[i].SetGeneratorFloor(newFloor);
            moves.push_back(GameState{ newFloor, modifiedObjectState, m_moves + 1 });
         }

         if (chipOnFloor) {
            {
               // We can move the chip
               ObjectState modifiedObjectState{ m_objectState };
               modifiedObjectState[i].SetChipFloor(newFloor);
               if (ObjectStateIsValid(modifiedObjectState)) {
                  moves.push_back(GameState{ newFloor, modifiedObjectState, m_moves + 1 });
               }
            }

            // Now consider moving a pair of chips
            for (size_t j = i + 1; j < m_objectState.size(); ++j) {
               if (m_currentFloor == m_objectState[j].GetChipFloor()) {
                  // Can maybe move both of these chips.
                  ObjectState modifiedObjectState{ m_objectState };
                  modifiedObjectState[i].SetChipFloor(newFloor);
                  modifiedObjectState[j].SetChipFloor(newFloor);
                  if (ObjectStateIsValid(modifiedObjectState)) {
                     moves.push_back(GameState{ newFloor, modifiedObjectState, m_moves + 1 });
                  }
               }
            }
         }

         if (generatorOnFloor) {
            {
               // We can move the generator
               ObjectState modifiedObjectState{ m_objectState };
               modifiedObjectState[i].SetGeneratorFloor(newFloor);
               if (ObjectStateIsValid(modifiedObjectState)) {
                  moves.push_back(GameState{ newFloor, modifiedObjectState, m_moves + 1 });
               }
            }

            // Now consider moving a pair of generators
            for (size_t j = i + 1; j < m_objectState.size(); ++j) {
               if (m_currentFloor == m_objectState[j].GetGeneratorFloor()) {
                  // Can maybe move both of these generators.
                  ObjectState modifiedObjectState{ m_objectState };
                  modifiedObjectState[i].SetGeneratorFloor(newFloor);
                  modifiedObjectState[j].SetGeneratorFloor(newFloor);
                  if (ObjectStateIsValid(modifiedObjectState)) {
                     moves.push_back(GameState{ newFloor, modifiedObjectState, m_moves + 1 });
                  }
               }
            }
         }
      }
   }

   return moves;

   //std::vector<Object> currentFloorObjects{ m_floorState[m_currentFloor].GetAllObjects() };
   //std::vector<std::vector<Object>> objectCombinations = Combinations(currentFloorObjects, 1);
   ////bool canMoveOneObject{ objectCombinations.size() > 0 };
   //std::vector<std::vector<Object>> twoObjectCombinations = Combinations(currentFloorObjects, 2);
   ////bool canMoveTwoObjects{ twoObjectCombinations.size() > 0 };
   //objectCombinations.insert(std::end(objectCombinations), std::begin(twoObjectCombinations), std::end(twoObjectCombinations));

   //for (const std::vector<Object>& objectsToRemove : objectCombinations) {
   //   if (2 == objectsToRemove.size()) {
   //      Object o1{ objectsToRemove[0] }, o2{ objectsToRemove[1] };
   //      // A chip and a generator can't move on the elevator unless they are paired.
   //      if (o1.GetType() != o2.GetType() && o1.GetMaterial() != o2.GetMaterial()) {
   //         continue;
   //      }
   //   }

   //   Floor newCurrentFloor{ m_floorState[m_currentFloor].RemoveObjects(objectsToRemove) };
   //   if (!newCurrentFloor.IsValid()) {
   //      continue;
   //   }

   //   for (const unsigned int newFloor : newFloors) {
   //      //bool movingDownstairs{ newFloor < m_currentFloor };

   //      //// If moving downstairs, don't bring two objects when you can bring one
   //      //if (movingDownstairs && canMoveOneObject && 2 == objectsToRemove.size()) {
   //      //   continue;
   //      //}

   //      //// If moving upstairs, don't bring one object when you can bring two
   //      //if (!movingDownstairs && canMoveTwoObjects && 1 == objectsToRemove.size()) {
   //      //   continue;
   //      //}

   //      Floor newFloorContents{ m_floorState[newFloor].AddObjects(objectsToRemove) };

   //      if (!newFloorContents.IsValid()) {
   //         continue;
   //      }

   //      FloorState modifiedFloorState{ m_floorState };
   //      modifiedFloorState[m_currentFloor] = newCurrentFloor;
   //      modifiedFloorState[newFloor] = newFloorContents;

   //      moves.push_back(GameState{ newFloor, modifiedFloorState, m_moves + 1 });
   //   }
   //}

   //return moves;
}
