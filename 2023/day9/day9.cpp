#include <iostream>

#include "cxxopts.hpp"
#include "input_handler.h"

long long predict(const std::vector<long long>& sequence)
{
  if (std::all_of(sequence.begin(), sequence.end(), [](const auto n){ return n == 0; }))
  {
    return 0;
  }

  std::vector<long long> new_sequence;
  for (size_t index = 1; index < sequence.size(); ++index)
  {
    new_sequence.push_back(sequence[index] - sequence[index - 1]);
  }

  long long next_value = predict(new_sequence);
  return next_value + *sequence.rbegin();
}

long long predict_back(const std::vector<long long>& sequence)
{
  if (std::all_of(sequence.begin(), sequence.end(), [](const auto n){ return n == 0; }))
  {
    return 0;
  }

  std::vector<long long> new_sequence;
  for (size_t index = 1; index < sequence.size(); ++index)
  {
    new_sequence.push_back(sequence[index] - sequence[index - 1]);
  }

  long long previous_value = predict_back(new_sequence);
  return *sequence.begin() - previous_value;
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d9", "Day 9 of Advent of Code");
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
  std::vector<std::string> lines{ input.read_all_lines() };
  long long forward_sum{0};
  long long backward_sum{0};
  for (const std::string& line : lines)
  {
    std::vector<std::string> tokens = advent_of_code::tokenize(line, ' ');
    std::vector<long long> numbers;
    std::transform(tokens.begin(), tokens.end(), std::back_inserter(numbers), [](const auto& s){ return std::stoll(s); });

    long long next_value = predict(numbers);
    forward_sum += next_value;

    next_value = predict_back(numbers);
    backward_sum += next_value;
  }

  std::cout << "Extrapolated forward sum: " << forward_sum << std::endl;
  std::cout << "Extrapolated backward sum: " << backward_sum << std::endl;
  return 0;
}
