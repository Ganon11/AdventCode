#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "broadcast_module.h"
#include "conjunction_module.h"
#include "cxxopts.hpp"
#include "flip_flop_module.h"
#include "input_handler.h"
#include "module.h"

namespace
{
typedef std::map<std::string, std::shared_ptr<Module>> ModuleMap;

ModuleMap parse_input(const std::vector<std::string>& lines, std::queue<SignalToProcess>& the_queue)
{
  ModuleMap modules;

  for (const std::string& line : lines)
  {
    std::vector<std::string> tokens{ advent_of_code::tokenize(line, " -> ") };
    std::string source_str{ tokens[0] };
    std::string destination_str{ tokens[1] };

    std::shared_ptr<Module> source;
    if (source_str == "broadcaster")
    {
      source = std::make_shared<BroadcastModule>(source_str, the_queue);
    }
    else if ('%' == source_str[0])
    {
      source = std::make_shared<FlipFlopModule>(source_str.substr(1), the_queue);
    }
    else if ('&' == source_str[0])
    {
      source = std::make_shared<ConjunctionModule>(source_str.substr(1), the_queue);
    }
    else
    {
      source = std::make_shared<Module>(source_str, the_queue);
    }

    tokens = advent_of_code::tokenize(destination_str, ", ");
    for (const std::string& destination : tokens)
    {
      if (0 == destination.compare("rx"))
      {
        //std::cout << "Adding rx destination to " << source->name() << std::endl;
      }
      source->add_destination(destination);
    }

    modules.insert(ModuleMap::value_type{ source->name(), source} );
  }

  // Hook up sources
  for (const auto& kvp : modules)
  {
    for (const std::string& destination : kvp.second->destinations())
    {
      auto itr{ modules.find(destination) };
      if (modules.end() == itr)
      {
        //std::cout << "Unknown module " << destination << ", skipping" << std::endl;
        continue;
      }

      itr->second->add_source(kvp.first);
    }
  }

  return modules;
}

bool push_the_button(const ModuleMap& modules, std::queue<SignalToProcess>& the_queue,
  unsigned long long& high_count, unsigned long long& low_count)
{
  the_queue.push(SignalToProcess{ LOW, "button", "broadcaster" });
  while (!the_queue.empty())
  {
    SignalToProcess process{ the_queue.front() };
    the_queue.pop();
    //std::cout << process.source << " -" << convert(process.signal) << "-> " << process.destination << std::endl;
    if (process.signal == HIGH)
    {
      ++high_count;
    }
    else
    {
      ++low_count;
    }

    if (0 == process.source.compare("rx") && process.signal == LOW)
    {
      return true;
    }

    auto itr = modules.find(process.destination);
    if (modules.end() == itr)
    {
      //std::cout << "Unknown module " << process.destination << ", skipping" << std::endl;
      continue;
    }

    itr->second->receive_signal(process.source, process.signal);
  }

  return false;
}
}

int main(int argc, char* argv[])
{
  cxxopts::Options options("d20", "Day 20 of Advent of Code");
  options.add_options()
    ("f,filename", "Input Filename", cxxopts::value<std::string>())
    ("h,help", "Print usage")
    ("c,count", "How many times to push the button", cxxopts::value<unsigned int>())
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

  unsigned int count{ 1 };
  if (result.count("count"))
  {
    count = result["count"].as<unsigned int>();
  }

  advent_of_code::InputHandler input{ result["filename"].as<std::string>() };
  std::queue<SignalToProcess> message_queue;
  ModuleMap modules{ parse_input(input.read_all_lines(), message_queue) };

  unsigned long long high_count{ 0 }, low_count{ 0 };
  int counter;
  for (auto counter = 0; counter < count; ++counter)
  {
    push_the_button(modules, message_queue, high_count, low_count);
  }

  std::cout << "Signal strength after " << count << ": " << high_count * low_count << std::endl;

  // while (!push_the_button(modules, message_queue, high_count, low_count))
  // {
  //   ++counter;
  // }

  // std::cout << "rx recieved after " << counter << " steps." << std::endl;

  return 0;
}
