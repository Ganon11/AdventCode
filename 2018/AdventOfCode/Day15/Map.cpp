#include "pch.h"
#include "Map.h"

#include <algorithm>
#include <functional>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

Map::Map(const advent_of_code::InputHandler& input) {
   unsigned int y{ 0 };
   std::vector<std::wstring> lines{ input.read_all_lines() };
   m_the_map.reserve(lines.size());
   for (const std::wstring& line : input.read_all_lines()) {
      std::vector<MapFeature> current_row;
      current_row.reserve(line.length());
      unsigned int x{ 0 };
      for (const wchar_t ch : line) {
         Position p{ x++, y };
         if (L'#' == ch) {
            current_row.push_back(WALL);
         } else {
            current_row.push_back(FLOOR);
            switch (ch) {
            case L'E':
               m_units.push_back(Unit::make_shared(p, ELF));
               break;
            case L'G':
               m_units.push_back(Unit::make_shared(p, GOBLIN));
               break;
            }
         }
      }

      m_the_map.push_back(current_row);
      ++y;
   }

   m_max_y = m_the_map.size() - 1;
   m_max_x = m_the_map[0].size() - 1;
}

bool Map::simulate_round() {
   std::sort(m_units.begin(), m_units.end(),
      [](const UnitPtr& u1, const UnitPtr& u2){ return u1->operator<(*u2); });

   std::set<unsigned int> dead_unit_ids;
   auto unit_is_dead{ [&dead_unit_ids](const UnitPtr& u)
      { return dead_unit_ids.end() != dead_unit_ids.find(u->get_id()); } };

   bool completed_round{ true };

   for (const UnitPtr& unit : m_units) {
      if (unit_is_dead(unit)) {
         // This unit is dead!
         continue;
      }

      Position current_position{ unit->get_position() };

      std::vector<UnitPtr> enemies;
      for (const UnitPtr& other : m_units) {
         if (unit != other
            && unit->get_type() != other->get_type()
            && !unit_is_dead(other)) {
            enemies.push_back(other);
         }
      }

      if (enemies.empty()) {
         // No non-dead enemies? Victory!
         completed_round = false;
         break;
      }

      // Am I adjacent to an enemy?
      std::vector<UnitPtr> adjacent_enemies;
      for (const UnitPtr& enemy : enemies) {
         if (unit->get_position().is_adjacent_to(enemy->get_position())) {
            adjacent_enemies.push_back(enemy);
         }
      }

      if (adjacent_enemies.empty()) {
         // Determine open neighboring squares
         std::set<Position> open_neighbors;
         for (const UnitPtr& enemy : enemies) {
            auto neighbors{ enemy->get_position().get_adjacent_positions() };
            for (const auto neighbor : neighbors) {
               if (position_is_open(neighbor)) {
                  open_neighbors.insert(neighbor);
               }
            }
         }

         if (open_neighbors.empty()) {
            // No open spots to move
            continue;
         }

         // Determine reachable squares and closest
         size_t closest{ std::numeric_limits<size_t>::max() };
         std::set<Position> candidate_positions;
         //Position new_position{ NONE_POSITION };
         for (const Position& open_neighbor : open_neighbors) {
            auto path{ shortest_path_between(current_position, open_neighbor) };
            // A* returns an empty path if the destination is unreachable
            if (path.size() == 0) {
               continue;
            }

            if (path.size() < closest) {
               closest = path.size();
               candidate_positions.clear();
               candidate_positions.insert(path[0]);
               //new_position = path[0];
            }
            else if (path.size() == closest) {
               candidate_positions.insert(path[0]);
            }
         }

         if (0 == candidate_positions.size()) {
            // Huh?
            continue;
         }
         
         size_t num_candidates{ candidate_positions.size() };
         unit->move(*(candidate_positions.begin()));

         for (const UnitPtr& enemy : enemies) {
            if (unit->get_position().is_adjacent_to(enemy->get_position())) {
               adjacent_enemies.push_back(enemy);
            }
         }
      }

      // Moving may have brought us next to an adjacent enemy - re-check!
      if (adjacent_enemies.empty()) {
         // No enemy, and we've already moved. Alas.
         continue;
      }

      // Find enemy to attack
      std::sort(adjacent_enemies.begin(), adjacent_enemies.end(),
         [](const UnitPtr& enemy1, const UnitPtr& enemy2) {
            if (enemy1->get_remaining_health() < enemy2->get_remaining_health()) {
               return true;
            }

            if (enemy1->get_remaining_health() == enemy2->get_remaining_health()) {
               return enemy1->get_position() < enemy2->get_position();
            }

            return false;
         }
      );

      if (adjacent_enemies[0]->take_hit(unit->get_attack_power())) {
         dead_unit_ids.insert(adjacent_enemies[0]->get_id());
      }
   }

   // Remove dead units
   if (dead_unit_ids.size() > 0) {
      auto dead_unit_finder{
          [&dead_unit_ids](const UnitPtr& u) {
              return dead_unit_ids.end() != dead_unit_ids.find(u->get_id());
          }
      };
      m_units.erase(std::remove_if(m_units.begin(), m_units.end(), dead_unit_finder), m_units.end());
   }

   return completed_round;
}

// Implementation of A*
std::vector<Position> Map::shortest_path_between(const Position& source, const Position& dest) const {
   // Heuristic is Manhattan Distance to destination
   auto heuristic{ [dest](const Position& p1, const Position& p2) {
      auto p1distance{ p1.distance_to(dest) };
      auto p2distance{ p2.distance_to(dest) };
      if (p1distance < p2distance) {
         return true;
      }

      if (p1distance == p2distance && p2 < p1) {
         return true;
      }

      return false;
   } };

   // Will sort positions by heuristic
   std::priority_queue<Position,
                       std::vector<Position>,
                       std::function<bool(Position, Position)>> frontier{ heuristic };

   // Reached key Position from value Position, allowing you to walk backwards from the destination to
   // the source
   std::map<Position, Position> came_from;

   // Used to revisit old Positions from shorter paths
   std::map<Position, size_t> cost_so_far;

   // The frontier starts as just the source
   frontier.push(source);

   // Free to reach source from source
   cost_so_far[source] = 0;

   while (!frontier.empty()) {
      // Pick a node from the frontier
      Position current{ frontier.top() };
      frontier.pop();

      // If we've reached our destination, we're done!
      if (dest == current) {
         break;
      }
      auto neighbors{ current.get_adjacent_positions() };
      for (const auto& next : neighbors) {
         // Trim neighbors
         if (!position_is_open(next)) {
            continue;
         }

         // Compare new_cost to see if we can do better than before
         size_t new_cost{ cost_so_far[current] + 1 };
         auto old_cost{ cost_so_far.find(next) };

         // If we've reached next faster than before (or for the first time)
         if (cost_so_far.end() == old_cost || new_cost < old_cost->second) {
            
            // Update cost
            cost_so_far[next] = new_cost;

            // Add next to the frontier, so that we can revisit it
            frontier.push(next);

            // Keep a record of how we reached this (current) shortest path
            came_from[next] = current;
         }
      }
   }

   // Walk came_from backwards to find source
   Position current{ dest };
   std::vector<Position> path;
   while (current != source) {
      auto from{ came_from.find(current) };
      if (came_from.end() == from) {
         break;
      }

      path.insert(path.begin(), from->first);
      current = from->second;
   }

   return path;
}

UnitType Map::victory_reached() const {
   if (std::all_of(m_units.begin(), m_units.end(), [](const UnitPtr& u) { return u->get_type() == ELF; })) {
      return ELF;
   }

   if (std::all_of(m_units.begin(), m_units.end(), [](const UnitPtr& u) { return u->get_type() == GOBLIN; })) {
      return GOBLIN;
   }

   return NONE;
}

unsigned int Map::get_total_remaining_health() const {
   unsigned int sum{ 0 };
   for (const UnitPtr& unit : m_units) {
      sum += unit->get_remaining_health();
   }

   return sum;
}

void Map::write_path(const std::vector<Position>& path) {
   for (const Position& p : path) {
      m_the_map[p.m_y][p.m_x] = PATH;
   }
}

void Map::clear_path() {
   for (size_t y = 0; y < m_the_map.size(); ++y) {
      for (size_t x = 0; x < m_the_map[y].size(); ++x) {
         if (m_the_map[y][x] == PATH) {
            m_the_map[y][x] = FLOOR;
         }
      }
   }
}

std::wostream& operator<<(std::wostream& out, const Map& m) {
   for (size_t y = 0; y < m.m_the_map.size(); ++y) {
      std::vector<UnitPtr> units_in_this_row;
      for (size_t x = 0; x < m.m_the_map[y].size(); ++x) {
         Position p{ x, y };
         auto unit = std::find_if(m.m_units.begin(), m.m_units.end(), [p](const UnitPtr& u) { return p == u->get_position(); });
         if (m.m_units.end() == unit) {
            switch (m.m_the_map[y][x]) {
            case WALL:
               out << L'#';
               break;
            case FLOOR:
               out << L'.';
               break;
            case PATH:
               out << L'x';
               break;
            }
         } else {
            units_in_this_row.push_back(*unit);
            switch ((*unit)->get_type()) {
            case ELF:
               out << L'E';
               break;
            case GOBLIN:
               out << L'G';
               break;
            }
         }
      }

      if (!units_in_this_row.empty()) {
         out << L'\t';
         for (size_t index = 0; index < units_in_this_row.size(); ++index) {
            if (index > 0) {
               out << L", ";
            }

            switch (units_in_this_row[index]->get_type()) {
            case ELF:
               out << L'E';
               break;
            case GOBLIN:
               out << L'G';
               break;
            }

            out << L'(' << units_in_this_row[index]->get_remaining_health() << L')';
         }
      }
      out << std::endl;
   }

   return out;
}

bool Map::position_is_open(const Position& p) const {
   auto unit_finder{ [p](const UnitPtr& u) { return p == u->get_position() && !u->is_dead(); } };
   return m_the_map[p.m_y][p.m_x] != WALL // Can't walk into a wall
      && !std::any_of(m_units.begin(), m_units.end(), unit_finder); // Can't walk into a unit
}
