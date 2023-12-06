#include <algorithm>
#include <cctype>
#include <iostream>
#include <numeric>
#include <vector>

#include "cxxopts.hpp"
#include "input_handler.h"

namespace 
{
class Race
{
public:
  Race() = default;
  Race(unsigned long long t, unsigned long long d) : m_time{ t }, m_distance_to_beat{ d }
  { }

  unsigned long long time() const { return m_time; }
  unsigned long long distance() const { return m_distance_to_beat; }

private:
  unsigned long long m_time;
  unsigned long long m_distance_to_beat;
};

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

Race parse_single_race(const std::vector<std::string>& lines)
{
  std::vector<std::string> tokens = advent_of_code::tokenize(lines[0], ": ");
  std::string time_str = tokens[1];
  std::cout << "Time str pre removal: " << time_str << std::endl;
  time_str.erase(std::remove(time_str.begin(), time_str.end(), ' '), time_str.end());
  std::cout << "Time str post removal: " << time_str << std::endl;
  unsigned long long time = std::stoull(time_str);
  std::cout << "Parsed time: " << time << "ms" << std::endl;

  tokens = advent_of_code::tokenize(lines[1], ": ");
  std::string distance_str = tokens[1];
  std::cout << "Distance str pre removal: " << distance_str << std::endl;
  distance_str.erase(std::remove(distance_str.begin(), distance_str.end(), ' '), distance_str.end());
  std::cout << "Distance str post removal: " << distance_str << std::endl;
  unsigned long long distance = std::stoull(distance_str);
  std::cout << "Parsed distance: " << distance << std::endl;

  return Race{ time, distance };
}

std::vector<unsigned long long> generate_triangle_row(const unsigned long long total_time)
{
  std::vector<unsigned long long> triangle;
  //for (unsigned long long time_held = 0; time_held <= (total_time / 2); ++time_held)
  for (unsigned long long time_held = 0; time_held <= total_time; ++time_held)
  {
    triangle.push_back(time_held * (total_time - time_held));
    //std::cout << "Holding for " << time_held << "ms results in distance " << triangle[time_held] << std::endl;
  }

  // if (total_time % 2 == 0)
  // {
  //   triangle.push_back((total_time / 2) * (total_time / 2));
  // }

  return triangle;
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d6", "Day 6 of Advent of Code");
  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
  ;

  auto result = options.parse(argc, argv);
  if (!result.count("filename"))
  {
    return -1;
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::vector<std::string> lines{ input.read_all_lines() };
  std::vector<Race> races = parse_races(lines);

  std::vector<unsigned long long> ways_to_win;
  for (const auto& race : races)
  {
    std::vector<unsigned long long> triangle = generate_triangle_row(race.time());
    unsigned long long ways = std::count_if(triangle.begin(), triangle.end(), [&race](const unsigned long long t){ return t > race.distance(); });
    // if (race.time() % 2 == 0)
    // {
    //   ++ways;
    // }

    //std::cout << "There are " << ways << " ways to beat distance " << race.distance() << " in " << race.time() << "ms" << std::endl;

    ways_to_win.push_back(ways);
  }

  unsigned long long product = std::reduce(ways_to_win.begin(), ways_to_win.end(), 1, [](const unsigned long long a, const unsigned long long b) { return a * b; });
  std::cout << "Win-ways product: " << product << std::endl;

  Race big_race = parse_single_race(lines);
  std::cout << "Big race of time " << big_race.time() << "ms, distance to beat " << big_race.distance() << std::endl;
  std::vector<unsigned long long> triangle = generate_triangle_row(big_race.time());
  unsigned long long ways = std::count_if(triangle.begin(), triangle.end(), [&big_race](const unsigned long long t){ return t > big_race.distance(); });

  std::cout << "Big race ways: " << ways << std::endl;

  return 0;
}
