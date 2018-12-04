#include "pch.h"
#include "Command.h"
#include <regex>

// ####################################################################################################################
// # Command
// ####################################################################################################################

Command::Command(const std::wstring& line)
{
   std::wregex timestamp_regex{ L"\\[(\\d+)-(\\d+)-(\\d+) (\\d+):(\\d+)\\]" };
   std::wsmatch matches;
   std::regex_search(line, matches, timestamp_regex);
   if (matches.size() == 6) // Whole match, then year-month-date hour:minute
   {
      m_year = _wtoi(matches[1].str().c_str());
      m_month = _wtoi(matches[2].str().c_str());
      m_date = _wtoi(matches[3].str().c_str());
      m_hour = _wtoi(matches[4].str().c_str());
      m_minute = _wtoi(matches[5].str().c_str());
   }
}

unsigned int Command::get_year() const
{
   return m_year;
}

unsigned int Command::get_month() const
{
   return m_month;
}

unsigned int Command::get_date() const
{
   return m_date;
}

unsigned int Command::get_hour() const
{
   return m_hour;
}

unsigned int Command::get_minute() const
{
   return m_minute;
}

bool Command::operator<(const Command& other) const
{
   unsigned int other_year{other.get_year()};
   if (m_year < other_year)
   {
      return true;
   }
   else if (m_year > other_year)
   {
      return false;
   }

   unsigned int other_month{ other.get_month() };
   if (m_month < other_month)
   {
      return true;
   }
   else if (m_month > other_month)
   {
      return false;
   }

   unsigned int other_date{ other.get_date() };
   if (m_date < other_date)
   {
      return true;
   }
   else if (m_date > other_date)
   {
      return false;
   }

   unsigned int other_hour{ other.get_hour() };
   if (m_hour < other_hour)
   {
      return true;
   }
   else if (m_hour > other_hour)
   {
      return false;
   }

   unsigned int other_minute{ other.get_minute() };
   if (m_minute < other_minute)
   {
      return true;
   }

   return false;
}

std::shared_ptr<Command> Command::make_shared(const std::wstring& line)
{
   std::wregex start_shift_regex{ L".*Guard.*" };
   std::wregex wakes_up_regex{ L".*wakes up.*" };
   std::wregex falls_asleep_regex{ L".*falls asleep.*" };

   if (std::regex_match(line, start_shift_regex))
   {
      return std::make_shared<StartShiftCommand>(line);
   }

   if (std::regex_match(line, wakes_up_regex))
   {
      return std::make_shared<WakesUpCommand>(line);
   }

   if (std::regex_match(line, falls_asleep_regex))
   {
      return std::make_shared<FallsAsleepCommand>(line);
   }

   return nullptr;
}

bool Command::sort(std::shared_ptr<Command> a, std::shared_ptr<Command> b)
{
   return (*a) < (*b);
}

// ####################################################################################################################
// # StartShiftCommand
// ####################################################################################################################

StartShiftCommand::StartShiftCommand(const std::wstring& line)
   : Command(line)
{
   std::wregex guard_id_regex{ L"Guard #(\\d+) begins shift" };
   std::wsmatch matches;
   std::regex_search(line, matches, guard_id_regex);
   if (matches.size() == 2) // Whole match, then id
   {
      m_id = _wtoi(matches[1].str().c_str());
   }
}

CommandType StartShiftCommand::get_command_type() const
{
   return GUARD_STARTS_SHIFT;
}

unsigned int StartShiftCommand::get_id() const
{
   return m_id;
}

// ####################################################################################################################
// # FallsAsleepCommand
// ####################################################################################################################

FallsAsleepCommand::FallsAsleepCommand(const std::wstring& line)
   : Command(line)
{
}

CommandType FallsAsleepCommand::get_command_type() const
{
   return GUARD_FALLS_ASLEEP;
}

// ####################################################################################################################
// # WakesUpCommand
// ####################################################################################################################

WakesUpCommand::WakesUpCommand(const std::wstring& line)
   : Command(line)
{
}

CommandType WakesUpCommand::get_command_type() const
{
   return GUARD_WAKES_UP;
}
