#pragma once

#include <limits>
#include "Task.h"

class Worker
{
public:
   Worker();

   bool is_busy(const unsigned int timer) const;
   void assign_task(const Task& t, const unsigned int timer);
   bool has_completed_task(const unsigned int timer) const;
   void deassign_task();
   bool is_currently_assigned_task() const;
   wchar_t get_current_task() const;

private:
   const static wchar_t UNASSIGNED_TASK{ std::numeric_limits<wchar_t>::max() };

   wchar_t m_current_task;
   unsigned int m_current_task_time;
   unsigned int m_start_time;
};
