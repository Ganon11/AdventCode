#pragma once

#include <map>
#include <string>
#include <vector>

#include "position.h"
#include "ray.h"

enum GridType
{
  SPACE,
  FORWARD_MIRROR,
  BACKWARD_MIRROR,
  VERTICAL_SPLITTER,
  HORIZONTAL_SPLITTER
};

char convert(const GridType type);
GridType convert(const char ch);

typedef std::map<advent_of_code::Position, GridType> Grid;

Grid parse_grid(const std::vector<std::string>& lines);
advent_of_code::Position find_terminating_position(const Grid& grid, const Ray& ray, bool& went_out_of_bounds);
std::vector<Ray> get_resultant_rays(const Ray& ray, const advent_of_code::Position& position, const GridType type);
