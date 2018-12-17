#pragma once

#include <iostream>
#include <limits>
#include <vector>

namespace advent_of_code {
struct Position
{
   Position() = default;
   Position(const long long x, const long long y);
   Position(const Position& p) = default;
   bool operator<(const Position& other) const;
   bool operator==(const Position& other) const;
   bool operator!=(const Position& other) const;
   bool is_adjacent_to(const Position& other) const;

   std::vector<Position> get_adjacent_positions() const;
   size_t distance_to(const Position& other) const;

   friend std::wostream& operator<<(std::wostream& out, const Position& t);

   long long m_x;
   long long m_y;
};

#ifdef max
#undef max
#endif

const Position NONE_POSITION{
    std::numeric_limits<long long>::max(), std::numeric_limits<long long>::max() };
}
