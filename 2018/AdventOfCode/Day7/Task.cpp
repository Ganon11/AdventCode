#include "pch.h"
#include "Task.h"

#include "../AoCHelpers/Sample.h"

Task::Task(const wchar_t name) : m_task_name{ name }
{ }

wchar_t Task::get_name() const
{
   return m_task_name;
}

void Task::add_dependency(const wchar_t dependency)
{
   m_dependencies.insert(dependency);
}

const std::set<wchar_t>& Task::get_dependencies() const
{
   return m_dependencies;
}

bool Task::operator==(const Task& other) const
{
   return m_task_name == other.get_name();
}

bool Task::operator<(const Task& other) const
{
   return m_task_name < other.get_name();
}

unsigned int Task::task_length() const
{
   unsigned int length{ static_cast<unsigned int>(m_task_name - L'A' + 1) };
   if (!AdventOfCode::Sample::USE_SAMPLE)
   {
      length += 60;
   }

   return length;
}

bool Task::is_available(const std::set<wchar_t>& completed_tasks) const
{
   if (0 == m_dependencies.size()) // No dependencies
   {
      return true;
   }

   for (const wchar_t dependency : m_dependencies)
   {
      if (completed_tasks.end() == completed_tasks.find(dependency))
      {
         return false;
         break;
      }
   }

   return true;
}
