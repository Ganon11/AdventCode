#include <algorithm>
#include <cstddef>
#include <iostream>

#include "conversion_range.h"
#include "cxxopts.hpp"
#include "input_handler.h"
#include "range.h"

namespace
{
std::vector<ConversionRange> parse_ranges(const std::vector<std::string>& lines, size_t& line_index)
{
  ++line_index; // Blank line
  ++line_index; // label
  std::vector<ConversionRange> ranges;
  do {
    ranges.push_back(ConversionRange{ lines[line_index++] });
  } while (!lines[line_index].empty());

  return ranges;
}
void parse_conversion_ranges(const std::vector<std::string>& lines,
  std::vector<ConversionRange>& seed_to_soil_ranges, std::vector<ConversionRange>& soil_to_fertilizer_ranges,
  std::vector<ConversionRange>& fertilizer_to_water_ranges, std::vector<ConversionRange>& water_to_light_ranges,
  std::vector<ConversionRange>& light_to_temperature_ranges, std::vector<ConversionRange>& temperature_to_humidity_ranges,
  std::vector<ConversionRange>& humidity_to_location_ranges)
{
  size_t line_index{2};
  seed_to_soil_ranges = parse_ranges(lines, line_index);
  soil_to_fertilizer_ranges = parse_ranges(lines, line_index);
  fertilizer_to_water_ranges = parse_ranges(lines, line_index);
  water_to_light_ranges = parse_ranges(lines, line_index);
  light_to_temperature_ranges = parse_ranges(lines, line_index);
  temperature_to_humidity_ranges = parse_ranges(lines, line_index);
  humidity_to_location_ranges = parse_ranges(lines, line_index);
}

unsigned long long convert(const std::vector<ConversionRange>& ranges, const unsigned long long source)
{
  for (const auto& range : ranges)
  {
    if (range.contains(source))
    {
      return range.convert(source);
    }
  }

  return source;
}

std::vector<unsigned long long> convert(const std::vector<ConversionRange>& ranges, const std::vector<unsigned long long>& sources)
{
  std::vector<unsigned long long> converted;
  converted.reserve(sources.size());
  for (const auto source : sources)
  {
    converted.push_back(convert(ranges, source));
  }

  return converted;
}

std::vector<unsigned long long> parse_seeds(const std::string& line)
{
  std::vector<std::string> tokens = advent_of_code::tokenize(line, ": ");
  std::vector<unsigned long long> seeds;
  for (const auto& token : advent_of_code::tokenize(tokens[1], ' '))
  {
    seeds.push_back(std::stoull(token));
  }

  return seeds;
}

void parse_part_1(const std::vector<std::string>& lines, std::vector<unsigned long long>& seeds,
  std::vector<ConversionRange>& seed_to_soil_ranges, std::vector<ConversionRange>& soil_to_fertilizer_ranges,
  std::vector<ConversionRange>& fertilizer_to_water_ranges, std::vector<ConversionRange>& water_to_light_ranges,
  std::vector<ConversionRange>& light_to_temperature_ranges, std::vector<ConversionRange>& temperature_to_humidity_ranges,
  std::vector<ConversionRange>& humidity_to_location_ranges)
{
  seeds = parse_seeds(lines[0]);
  parse_conversion_ranges(lines, seed_to_soil_ranges, soil_to_fertilizer_ranges, fertilizer_to_water_ranges,
    water_to_light_ranges, light_to_temperature_ranges, temperature_to_humidity_ranges, humidity_to_location_ranges);
}

std::vector<Range> parse_seed_ranges(const std::string& line)
{
  std::vector<std::string> tokens = advent_of_code::tokenize(line, ": ");
  std::string seed_range_values = tokens[1];
  tokens = advent_of_code::tokenize(seed_range_values, ' ');
  std::vector<Range> seed_ranges;
  for (size_t index = 0; index < tokens.size(); index += 2)
  {
    seed_ranges.push_back(Range{ std::stoull(tokens[index]), std::stoull(tokens[index + 1]) });
  }

  return seed_ranges;
}

std::vector<unsigned long long> convert_seed_ranges_to_seeds(const std::vector<Range>& ranges)
{
  std::vector<unsigned long long> seeds;
  for (const auto& range : ranges)
  {
    for (unsigned long long seed = range.start(); seed < range.start() + range.length(); ++seed)
    {
      seeds.push_back(seed);
    }
  }

  return seeds;
}

void parse_part_2(const std::vector<std::string>& lines, std::vector<unsigned long long>& seeds,
  std::vector<ConversionRange>& seed_to_soil_ranges, std::vector<ConversionRange>& soil_to_fertilizer_ranges,
  std::vector<ConversionRange>& fertilizer_to_water_ranges, std::vector<ConversionRange>& water_to_light_ranges,
  std::vector<ConversionRange>& light_to_temperature_ranges, std::vector<ConversionRange>& temperature_to_humidity_ranges,
  std::vector<ConversionRange>& humidity_to_location_ranges)
{
  seeds = convert_seed_ranges_to_seeds(parse_seed_ranges(lines[0]));
  parse_conversion_ranges(lines, seed_to_soil_ranges, soil_to_fertilizer_ranges, fertilizer_to_water_ranges,
    water_to_light_ranges, light_to_temperature_ranges, temperature_to_humidity_ranges, humidity_to_location_ranges);
}

std::vector<unsigned long long> do_all_conversions(const std::vector<unsigned long long>& seeds,
  const std::vector<ConversionRange>& seed_to_soil_ranges, const std::vector<ConversionRange>& soil_to_fertilizer_ranges,
  const std::vector<ConversionRange>& fertilizer_to_water_ranges, const std::vector<ConversionRange>& water_to_light_ranges,
  const std::vector<ConversionRange>& light_to_temperature_ranges, const std::vector<ConversionRange>& temperature_to_humidity_ranges,
  const std::vector<ConversionRange>& humidity_to_location_ranges)
{
  std::vector<unsigned long long> soils = convert(seed_to_soil_ranges, seeds);
  std::vector<unsigned long long> fertilizers = convert(soil_to_fertilizer_ranges, soils);
  std::vector<unsigned long long> waters = convert(fertilizer_to_water_ranges, fertilizers);
  std::vector<unsigned long long> lights = convert(water_to_light_ranges, waters);
  std::vector<unsigned long long> temperatures = convert(light_to_temperature_ranges, lights);
  std::vector<unsigned long long> humiditys = convert(temperature_to_humidity_ranges, temperatures);
  return convert(humidity_to_location_ranges, humiditys);
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d5", "Day 5 of Advent of Code");

  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("h,help", "Print usage")
  ;

  auto result = options.parse(argc, argv);
  if (result.count("help"))
  {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (!result.count("filename"))
  {
    std::cout << "ERROR: Missing required argument: <filename>" << std::endl;
    std::cout << options.help() << std::endl;
    return -1;
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::vector<std::string> lines{ input.read_all_lines(true) };
  std::vector<unsigned long long> seeds;
  std::vector<ConversionRange> seed_to_soil_ranges;
  std::vector<ConversionRange> soil_to_fertilizer_ranges;
  std::vector<ConversionRange> fertilizer_to_water_ranges;
  std::vector<ConversionRange> water_to_light_ranges;
  std::vector<ConversionRange> light_to_temperature_ranges;
  std::vector<ConversionRange> temperature_to_humidity_ranges;
  std::vector<ConversionRange> humidity_to_location_ranges;
  parse_part_1(lines, seeds, seed_to_soil_ranges, soil_to_fertilizer_ranges, fertilizer_to_water_ranges,
    water_to_light_ranges, light_to_temperature_ranges, temperature_to_humidity_ranges, humidity_to_location_ranges);

  std::vector<unsigned long long> locations = do_all_conversions(seeds, seed_to_soil_ranges,
    soil_to_fertilizer_ranges, fertilizer_to_water_ranges, water_to_light_ranges, light_to_temperature_ranges,
    temperature_to_humidity_ranges, humidity_to_location_ranges);

  unsigned long long min_location = *std::min_element(locations.begin(), locations.end());

  std::cout << "Part 1 Min location " << min_location << std::endl;

  parse_part_2(lines, seeds, seed_to_soil_ranges, soil_to_fertilizer_ranges, fertilizer_to_water_ranges,
    water_to_light_ranges, light_to_temperature_ranges, temperature_to_humidity_ranges, humidity_to_location_ranges);

  locations = do_all_conversions(seeds, seed_to_soil_ranges,
    soil_to_fertilizer_ranges, fertilizer_to_water_ranges, water_to_light_ranges, light_to_temperature_ranges,
    temperature_to_humidity_ranges, humidity_to_location_ranges);

  min_location = *std::min_element(locations.begin(), locations.end());

  std::cout << "Part 2 Min location " << min_location << std::endl;

  return 0;
}
