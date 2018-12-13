#include "pch.h"
#include "Worker.h"

Worker::Worker() :
  m_current_task{ UNASSIGNED_TASK },
  m_current_task_time{ 0 },
  m_start_time{ 0 }
{ }

void Worker::assign_task(const Task& t, const unsigned int timer) {
  m_current_task = t.get_name();
  m_current_task_time = t.task_length();
  m_start_time = timer;
}

bool Worker::has_completed_task(const unsigned int timer) const {
  return (timer >= m_start_time + m_current_task_time);
}

void Worker::deassign_task() {
  m_current_task = UNASSIGNED_TASK;
  m_current_task_time = 0;
  m_start_time = 0;
}

bool Worker::is_currently_assigned_task() const {
  return m_current_task != UNASSIGNED_TASK;
}

wchar_t Worker::get_current_task() const {
  return m_current_task;
}
