#pragma once

#include <memory>
#include <string>

enum CommandType
{
   GUARD_STARTS_SHIFT,
   GUARD_FALLS_ASLEEP,
   GUARD_WAKES_UP
};

class Command
{
public:
   virtual CommandType get_command_type() const = 0;
   virtual unsigned int get_year() const;
   virtual unsigned int get_month() const;
   virtual unsigned int get_date() const;
   virtual unsigned int get_hour() const;
   virtual unsigned int get_minute() const;
   virtual bool operator<(const Command& other) const;

   static std::shared_ptr<Command> make_shared(const std::wstring& line);
   static bool sort(std::shared_ptr<Command> a, std::shared_ptr<Command> b);

protected:
   Command(const std::wstring& line);

private:
   unsigned int m_year;
   unsigned int m_month;
   unsigned int m_date;
   unsigned int m_hour;
   unsigned int m_minute;
};

class StartShiftCommand : public Command
{
public:
   StartShiftCommand(const std::wstring& line);

   virtual CommandType get_command_type() const;

   unsigned int get_id() const;

private:
   unsigned int m_id;
};

class FallsAsleepCommand : public Command
{
public:
   FallsAsleepCommand(const std::wstring& line);

   virtual CommandType get_command_type() const;
};

class WakesUpCommand : public Command
{
public:
   WakesUpCommand(const std::wstring& line);

   virtual CommandType get_command_type() const;
};
