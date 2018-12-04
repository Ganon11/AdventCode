// Day4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Command.h"
#include "Guard.h"

using namespace std;

bool get_commands(std::vector<std::shared_ptr<Command>>& commands);
bool process_commands(const std::vector<std::shared_ptr<Command>>& commands, std::map<unsigned int, Guard>& guards);
unsigned int part_1_answer(const std::map<unsigned int, Guard>& guards);
unsigned int part_2_answer(const std::map<unsigned int, Guard>& guards);

int main()
{
   std::vector<std::shared_ptr<Command>> commands;
   if (!get_commands(commands))
   {
      std::wcerr << L"Failed to parse commands." << std::endl;
      return -1;
   }

   std::map<unsigned int, Guard> guards;
   if (!process_commands(commands, guards))
   {
      std::wcerr << L"Failed to process commands." << std::endl;
      return -2;
   }

   std::wcout << L"Part 1: " << part_1_answer(guards) << std::endl;
   std::wcout << L"Part 2: " << part_2_answer(guards) << std::endl;

   return 0;
}

bool get_commands(std::vector<std::shared_ptr<Command>>& commands)
{
   wifstream input{ L"Input.txt" };
   if (!input.good())
   {
      return false;
   }

   wstring line;
   do
   {
      getline(input, line);
      commands.push_back(Command::make_shared(line));
   } while (input.good());

   std::sort(commands.begin(), commands.end(), Command::sort);
   return true;
}

bool process_commands(const std::vector<std::shared_ptr<Command>>& commands, std::map<unsigned int, Guard>& guards)
{
   unsigned int current_guard{ std::numeric_limits<unsigned int>::max() };
   unsigned int start_minute{ std::numeric_limits<unsigned int>::max() };
   unsigned int end_minute{ std::numeric_limits<unsigned int>::max() };
   bool is_awake = true;
   for (const std::shared_ptr<Command>& command : commands)
   {
      switch (command->get_command_type())
      {
      case GUARD_STARTS_SHIFT:
      {
         std::shared_ptr<StartShiftCommand> shift_command{ std::dynamic_pointer_cast<StartShiftCommand>(command) };
         current_guard = shift_command->get_id();
         if (guards.end() == guards.find(current_guard))
         {
            guards[current_guard] = Guard(current_guard);
         }
         is_awake = true;
         break;
      }
      case GUARD_FALLS_ASLEEP:
      {
         std::shared_ptr<FallsAsleepCommand> asleep_command{ std::dynamic_pointer_cast<FallsAsleepCommand>(command) };
         if (!is_awake)
         {
            std::wcerr << L"Guard fell asleep while already asleep." << endl;
            return false;
         }

         start_minute = asleep_command->get_minute();
         is_awake = false;
         break;
      }
      case GUARD_WAKES_UP:
      {
         std::shared_ptr<WakesUpCommand> wakes_command{ std::dynamic_pointer_cast<WakesUpCommand>(command) };
         if (is_awake)
         {
            std::wcerr << L"Guard woke up while already awake." << endl;
            return false;
         }

         end_minute = wakes_command->get_minute();
         auto guard{ guards.find(current_guard) };
         if (guards.end() == guard)
         {
            std::wcerr << L"Couldn't find current guard." << endl;
            break;
         }

         guard->second.add_sleep(start_minute, end_minute);

         start_minute = std::numeric_limits<unsigned int>::max();
         end_minute = std::numeric_limits<unsigned int>::max();
         is_awake = true;
         break;
      }
      }
   }

   return true;
}

unsigned int part_1_answer(const std::map<unsigned int, Guard>& guards)
{
   auto sleepiest_guard = std::max_element(guards.begin(), guards.end(), [](const std::pair<unsigned int, Guard>& g1, const std::pair<unsigned int, Guard>& g2){ return g1.second.minutes_asleep() < g2.second.minutes_asleep(); });
   return sleepiest_guard->second.get_id() * sleepiest_guard->second.max_sleep_minute();
}

unsigned int part_2_answer(const std::map<unsigned int, Guard>& guards)
{
   auto sleepiest_guard = std::max_element(
      guards.begin(),
      guards.end(),
      [](const std::pair<unsigned int, Guard>& g1, const std::pair<unsigned int, Guard>& g2)
      {
         return g1.second.max_sleep_count() < g2.second.max_sleep_count();
      }
   );

   return sleepiest_guard->second.get_id() * sleepiest_guard->second.max_sleep_minute();
}
