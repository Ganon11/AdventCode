#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "Cart.h"
#include "Position.h"

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

  void tick();
  Position get_crash_position() const;
  void remove_carts(const Position& position);
  size_t num_carts() const;
  Position first_cart_position() const;

  friend std::wostream& operator<<(std::wostream& out, const Track& t);

private:
  std::vector<Cart> m_carts;
  std::map<Position, Path> m_tracks;
};
