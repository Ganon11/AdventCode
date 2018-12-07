#pragma once

#include <set>
#include <string>

class Task
{
public:
   Task(const wchar_t name);

   wchar_t get_name() const;
   void add_dependency(const wchar_t dependency);
   const std::set<wchar_t>& get_dependencies() const;

   bool operator==(const Task& other) const;
   bool operator<(const Task& other) const;

   unsigned int task_length() const;
   bool is_available(const std::set<wchar_t>& completed_tasks) const;

private:
   wchar_t m_task_name;
   std::set<wchar_t> m_dependencies;
};

