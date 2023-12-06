#include "race.h"
#include "input_handler.h"

Race::Race(unsigned long long t, unsigned long long d) : m_time{ t }, m_distance_to_beat{ d }
{ }

Race::Race(const std::vector<std::string>& lines)
{
  std::vector<std::string> tokens = advent_of_code::tokenize(lines[0], ": ");
  std::string time_str = tokens[1];
  time_str.erase(std::remove(time_str.begin(), time_str.end(), ' '), time_str.end());
  unsigned long long time = std::stoull(time_str);

  tokens = advent_of_code::tokenize(lines[1], ": ");
  std::string distance_str = tokens[1];
  distance_str.erase(std::remove(distance_str.begin(), distance_str.end(), ' '), distance_str.end());
  unsigned long long distance = std::stoull(distance_str);

  m_time = time;
  m_distance_to_beat = distance;
}

unsigned long long Race::time() const
{
  return m_time;
}

unsigned long long Race::distance() const
{
  return m_distance_to_beat;
}

unsigned long long Race::ways_to_win() const
{
  unsigned long long time_held;
  for (time_held = 0; time_held < m_time; ++time_held)
  {
    unsigned long long distance_traveled = time_held * (m_time - time_held);
    if (distance_traveled > m_distance_to_beat)
    {
      break;
    }
  }

  return m_time - (time_held * 2) + 1;
}

std::vector<Race> parse_races(const std::vector<std::string>& lines)
{
  std::vector<std::string> tokens = advent_of_code::tokenize(lines[0], ": ");
  std::vector<std::string> times_strings = advent_of_code::tokenize(tokens[1], ' ');
  tokens = advent_of_code::tokenize(lines[1], ": ");
  std::vector<std::string> distances_strings = advent_of_code::tokenize(tokens[1], ' ');

  std::vector<Race> races;
  races.reserve(times_strings.size());
  for (size_t index = 0; index < times_strings.size(); ++index)
  {
    races.push_back(Race{ std::stoull(times_strings[index]), std::stoull(distances_strings[index]) });
  }

  return races;
}
