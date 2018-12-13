#include "pch.h"
#include "Track.h"

#include <algorithm>
#include <set>

Track::Track(const advent_of_code::InputHandler& input) {
  unsigned int y{ 0 };
  for (const std::wstring& line : input.read_all_lines()) {
    unsigned int x{ 0 };
    for (size_t index = 0; index < line.length(); ++index) {
      Position p{ x, y };
      switch (line[index]) {
      case L'|':
        m_tracks[p] = NORTHSOUTH;
        break;
      case L'-':
        m_tracks[p] = EASTWEST;
        break;
      case L'/':
        m_tracks[p] = RIGHTBEND;
        break;
      case L'\\':
        m_tracks[p] = LEFTBEND;
        break;
      case L'+':
        m_tracks[p] = INTERSECTION;
        break;
      case '^':
        m_carts.push_back(Cart(p, NORTH));
        m_tracks[p] = NORTHSOUTH;
        break;
      case 'v':
        m_carts.push_back(Cart(p, SOUTH));
        m_tracks[p] = NORTHSOUTH;
        break;
      case '<':
        m_carts.push_back(Cart(p, WEST));
        m_tracks[p] = EASTWEST;
        break;
      case '>':
        m_carts.push_back(Cart(p, EAST));
        m_tracks[p] = EASTWEST;
        break;
      }

      ++x;
    }

    ++y;
  }
}

void Track::tick() {
  std::sort(m_carts.begin(), m_carts.end());
  for (Cart& cart : m_carts) {
    Position cart_position{ cart.move() };
    switch (m_tracks.at(cart_position)) {
    case RIGHTBEND:
      switch (cart.get_current_direction()) {
      case NORTH:
      case SOUTH:
        cart.turn_clockwise();
        break;
      case EAST:
      case WEST:
      default:
        cart.turn_counterclockwise();
        break;
      }
      break;
    case LEFTBEND:
      switch (cart.get_current_direction()) {
      case NORTH:
      case SOUTH:
        cart.turn_counterclockwise();
        break;
      case EAST:
      case WEST:
      default:
        cart.turn_clockwise();
        break;
      }
      break;
    case INTERSECTION:
      cart.turn();
      break;
    }
  }
}

Position Track::get_crash_position() const {
  std::set<Position> cart_positions;
  for (const Cart& cart : m_carts) {
    Position p{ cart.get_current_position() };
    if (cart_positions.end() != cart_positions.find(p)) {
      return p;
    }

    cart_positions.insert(p);
  }

  return NONE_POSITION;
}

void Track::remove_carts(const Position& position) {
  auto cart_finder{ [position](const Cart& c) { return position == c.get_current_position(); } };
  m_carts.erase(std::remove_if(m_carts.begin(), m_carts.end(), cart_finder), m_carts.end());
}

size_t Track::num_carts() const {
  return m_carts.size();
}

Position Track::first_cart_position() const {
  return m_carts[0].get_current_position();
}

std::wostream& operator<<(std::wostream& out, const Track& t) {
  auto x_comparator{ [](const std::map<Position, Path>::value_type& p1,
                        const std::map<Position, Path>::value_type& p2)
                        { return p1.first.m_x < p2.first.m_x; } };
  unsigned int max_x{
      std::max_element(t.m_tracks.begin(), t.m_tracks.end(), x_comparator)->first.m_x };

  auto y_comparator{ [](const std::map<Position, Path>::value_type& p1,
                        const std::map<Position, Path>::value_type& p2)
                        { return p1.first.m_y < p2.first.m_y; } };
  unsigned int max_y{
      std::max_element(t.m_tracks.begin(), t.m_tracks.end(), y_comparator)->first.m_y };

  for (unsigned int y = 0; y <= max_y; ++y) {
    for (unsigned int x = 0; x <= max_x; ++x) {
      Position p{ x, y };

      auto cart_finder{ [p](const Cart& c) { return p == c.get_current_position(); } };
      auto carts_itr{ std::find_if(t.m_carts.begin(), t.m_carts.end(), cart_finder) };
      if (t.m_carts.end() != carts_itr) {
        switch (carts_itr->get_current_direction()) {
        case NORTH:
          out << L'^';
          break;
        case EAST:
          out << L'>';
          break;
        case SOUTH:
          out << L'v';
          break;
        case WEST:
          out << L'<';
          break;
        }

        continue;
      }

      auto path_itr{ t.m_tracks.find(p) };
      if (t.m_tracks.end() != path_itr) {
        switch (path_itr->second) {
        case NORTHSOUTH:
          out << L'|';
          break;
        case EASTWEST:
          out << L'-';
          break;
        case RIGHTBEND:
          out << L'/';
          break;
        case LEFTBEND:
          out << L'\\';
          break;
        case INTERSECTION:
          out << L'+';
          break;
        }

        continue;
      }

      out << L' ';
    }

    out << std::endl;
  }

  return out;
}
