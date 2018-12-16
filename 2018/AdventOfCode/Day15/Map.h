#pragma once

#include "../AoCHelpers/InputHandler.h"
#include "Position.h"
#include "Unit.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>

enum MapFeature {
   WALL,
   FLOOR,
   PATH
};

typedef std::vector<std::vector<MapFeature>> TwoDimensionalMap;

class Map
{
public:
   Map(const advent_of_code::InputHandler& input);

   // Returns true if round completed
   bool simulate_round();
   std::vector<Position> shortest_path_between(const Position& from, const Position& to) const;
   UnitType victory_reached() const;
   unsigned int get_total_remaining_health() const;

   void write_path(const std::vector<Position>& path);
   void clear_path();

   friend std::wostream& operator<<(std::wostream& out, const Map& m);

private:
   bool position_is_open(const Position& p) const;

   TwoDimensionalMap m_the_map;
   std::vector<UnitPtr> m_units;
   size_t m_max_x;
   size_t m_max_y;
};
