#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "../AoCHelpers/Position.h"
#include "Cart.h"

enum Path
{
  NONE,
  NORTHSOUTH,
  EASTWEST,
  RIGHTBEND,
  LEFTBEND,
  INTERSECTION
};

class Track
{
public:
  Track(const advent_of_code::InputHandler& input);

  advent_of_code::Position tick();
  std::set<advent_of_code::Position> get_crash_positions() const;
  size_t num_carts() const;
  advent_of_code::Position first_cart_position() const;

  friend std::wostream& operator<<(std::wostream& out, const Track& t);

private:
  std::vector<Cart> m_carts;
  std::map<advent_of_code::Position, Path> m_tracks;
};
