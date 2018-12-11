#pragma once

#include <string>
#include <vector>

using namespace std;

class Claim
{
public:
  Claim(const wstring& line);

  unsigned int get_id() const;
  unsigned int get_left() const;
  unsigned int get_top() const;
  unsigned int get_width() const;
  unsigned int get_height() const;

  typedef pair<unsigned int, unsigned int> Coordinate;

  vector<Coordinate> generate_coordinates() const;

private:
  unsigned int m_id;
  unsigned int m_left;
  unsigned int m_top;
  unsigned int m_width;
  unsigned int m_height;
};