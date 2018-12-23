// Day23.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <iostream>
#include <regex>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "../AoCHelpers/Position.h"

struct Nanobot
{
  Nanobot(const advent_of_code::Position& p, const unsigned long long signal_radius);
  Nanobot(const long long x,
    const long long y,
    const long long z,
    const unsigned long long signal_radius);

  bool in_range(const advent_of_code::Position& p) const;

  advent_of_code::Position m_position;
  unsigned long long m_signal_radius;
};

int wmain(int argc, wchar_t* argv[]) {
  std::wregex nanobot_regex{ L"pos=<(-?\\d+),(-?\\d+),(-?\\d+)>, r=(-?\\d+)" };
  std::wsmatch matches;
  advent_of_code::InputHandler input{ argc, argv };
  std::vector<Nanobot> nanobots;

  for (const std::wstring& line : input.read_all_lines()) {
    if (!std::regex_search(line, matches, nanobot_regex) || matches.size() != 5) {
      std::wcerr << L"Malformed input" << std::endl;
      return -1;
    }

    long long x = static_cast<long long>(_wtoi(matches[1].str().c_str()));
    long long y = static_cast<long long>(_wtoi(matches[2].str().c_str()));
    long long z = static_cast<long long>(_wtoi(matches[3].str().c_str()));
    unsigned long long r = static_cast<unsigned long long>(_wtoi(matches[4].str().c_str()));
    nanobots.push_back(Nanobot{ x, y, z, r });
  }

  Nanobot strongest{ *std::max_element(nanobots.begin(),
    nanobots.end(),
    [](const Nanobot& b1, const Nanobot& b2) { return b1.m_signal_radius < b2.m_signal_radius; }) };

  auto nanobots_in_range{ std::count_if(nanobots.begin(), nanobots.end(), [&strongest](const Nanobot& b) {
    return strongest.m_position.distance_to(b.m_position) <= strongest.m_signal_radius;
  }) };

  std::wcout << nanobots_in_range << L" nanobots in range." << std::endl;

  advent_of_code::Position closest_position{ advent_of_code::NONE_POSITION };
  int max_nanobots_in_range{ 0 };

  auto x_comparator{ [](const Nanobot& b1, const Nanobot& b2) {
    return b1.m_position.m_x < b2.m_position.m_x;
  } };
  auto y_comparator{ [](const Nanobot& b1, const Nanobot& b2) {
    return b1.m_position.m_y < b2.m_position.m_y;
  } };
  auto z_comparator{ [](const Nanobot& b1, const Nanobot& b2) {
    return b1.m_position.m_z < b2.m_position.m_z;
  } };

  long long min_x{ std::min_element(nanobots.begin(), nanobots.end(), x_comparator)->m_position.m_x };
  long long min_y{ std::min_element(nanobots.begin(), nanobots.end(), y_comparator)->m_position.m_y };
  long long min_z{ std::min_element(nanobots.begin(), nanobots.end(), z_comparator)->m_position.m_z };

  long long max_x{ std::max_element(nanobots.begin(), nanobots.end(), x_comparator)->m_position.m_x };
  long long max_y{ std::max_element(nanobots.begin(), nanobots.end(), y_comparator)->m_position.m_y };
  long long max_z{ std::max_element(nanobots.begin(), nanobots.end(), z_comparator)->m_position.m_z };

  for (long long x = min_x; x <= max_x; ++x) {
    for (long long y = min_y; y <= max_y; ++y) {
      for (long long z = min_z; z <= max_z; ++z) {
        advent_of_code::Position current{ x, y, z };
        auto nanobot_counter{ [&current](const Nanobot& n) {
          return n.m_position.distance_to(current) <= n.m_signal_radius;
        } };
        int in_range{ std::count_if(nanobots.begin(), nanobots.end(), nanobot_counter) };
        if (in_range > max_nanobots_in_range) {
          max_nanobots_in_range = in_range;
          closest_position = current;
        } else if (in_range == max_nanobots_in_range
          && current.distance_to(advent_of_code::ORIGIN) < closest_position.distance_to(advent_of_code::ORIGIN)) {
          closest_position = current;
        }
      }
    }
  }

  std::wcout << L"Distance to optimal point is "
    << closest_position.distance_to(advent_of_code::ORIGIN) << std::endl;

  return 0;
}

Nanobot::Nanobot(const advent_of_code::Position& p, const unsigned long long signal_radius)
  : m_position{ p }, m_signal_radius{ signal_radius }
{}

Nanobot::Nanobot(const long long x,
  const long long y,
  const long long z,
  const unsigned long long signal_radius)
  : m_position{ x, y, z }, m_signal_radius{ signal_radius }
{}

bool Nanobot::in_range(const advent_of_code::Position& p) const {
  return m_position.distance_to(p) <= m_signal_radius;
}
