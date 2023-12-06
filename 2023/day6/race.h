#pragma once

#include <string>
#include <vector>

class Race
{
public:
  Race() = default;
  Race(unsigned long long t, unsigned long long d);
  Race(const std::vector<std::string>& lines);

  unsigned long long time() const;
  unsigned long long distance() const;
  unsigned long long ways_to_win() const;

private:
  unsigned long long m_time;
  unsigned long long m_distance_to_beat;
};

std::vector<Race> parse_races(const std::vector<std::string>& lines);
