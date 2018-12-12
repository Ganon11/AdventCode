// Day7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "Task.h"
#include "Worker.h"

#include "../AoCHelpers/InputHandler.h"
#include "../AoCHelpers/Sample.h"

bool get_tasks(std::vector<Task>& tasks, const AdventOfCode::InputHandler& input);
Task find_next_task(const std::vector<Task>& tasks, const std::set<wchar_t> completed_tasks);
std::vector<Task> sort_tasks(const std::vector<Task>& tasks);
void print_tasks_in_order(const std::vector<Task>& tasks);
unsigned int perform_tasks(const std::vector<Task>& tasks_in_order);

int wmain(int argc, wchar_t* argv[]) {
  AdventOfCode::InputHandler input{ argc, argv };
  std::vector<Task> tasks;
  if (!get_tasks(tasks, input)) {
    std::wcerr << L"Couldn't parse input." << std::endl;
    return -1;
  }

  std::vector<Task> tasks_in_order{ sort_tasks(tasks) };
  print_tasks_in_order(tasks_in_order);

  unsigned int duration{ perform_tasks(tasks_in_order) };
  std::wcout << L"All tasks complete in " << duration << L" seconds." << std::endl;

  return 0;
}

bool get_tasks(std::vector<Task>& tasks, const AdventOfCode::InputHandler& input) {
  std::wregex task_regex{ L"Step (\\w) must be finished before step (\\w) can begin." };
  std::wsmatch matches;
  for (const std::wstring& line : input.read_all_lines()) {
    std::regex_search(line, matches, task_regex);
    if (matches.size() != 3) // Whole match, dependency and task
    {
      std::wcerr << L"Error parsing line \"" << line << L"\"" << std::endl;
      return false;
    }

    wchar_t task_name = matches[2].str()[0];
    wchar_t dependency = matches[1].str()[0];

    // Add indicated task
    std::vector<Task>::iterator itr{ find(tasks.begin(), tasks.end(), task_name) };
    if (tasks.end() == itr) {
      Task t{ task_name };
      t.add_dependency(dependency);
      tasks.push_back(t);
    } else {
      itr->add_dependency(dependency);
    }

    // Add indicated dependency
    itr = find(tasks.begin(), tasks.end(), dependency);
    if (tasks.end() == itr) {
      Task t{ dependency };
      tasks.push_back(t);
    }
  }

  sort(tasks.begin(), tasks.end());

  return true;
}

Task find_next_task(const std::vector<Task>& tasks, const std::set<wchar_t> completed_tasks) {
  for (const Task& t : tasks) {
    if (completed_tasks.end() != completed_tasks.find(t.get_name())) {
      continue; // Already done this task.
    }

    if (t.is_available(completed_tasks)) {
      return t;
    }
  }

  return Task{ std::numeric_limits<wchar_t>::max() };
}

std::vector<Task> sort_tasks(const std::vector<Task>& tasks) {
  std::vector<Task> tasks_in_order;
  tasks_in_order.reserve(tasks.size());
  std::set<wchar_t> completed_tasks;
  while (completed_tasks.size() != tasks.size()) {
    Task next_task{ find_next_task(tasks, completed_tasks) };
    tasks_in_order.push_back(next_task);
    completed_tasks.insert(next_task.get_name());
  }

  return tasks_in_order;
}

void print_tasks_in_order(const std::vector<Task>& tasks) {
  std::wcout << L"Tasks in order: ";
  for (const Task& t : tasks) {
    std::wcout << t.get_name();
  }
  std::wcout << std::endl;
}

unsigned int perform_tasks(const std::vector<Task>& tasks_in_order) {
  const size_t NUM_WORKERS{ AdventOfCode::Sample::should_use_sample() ? 2u : 5u };

  std::vector<Worker> workers;
  workers.reserve(NUM_WORKERS);
  for (size_t index = 0; index < NUM_WORKERS; ++index) {
    workers.push_back(Worker{});
  }

  unsigned int timer{ 0 };
  std::set<wchar_t> completed_tasks;
  std::set<wchar_t> assigned_tasks;
  while (completed_tasks.size() != tasks_in_order.size()) {
    // Check if any assigned tasks can be completed
    for (Worker& w : workers) {
      if (w.is_currently_assigned_task() && w.has_completed_task(timer)) {
        completed_tasks.insert(w.get_current_task());
        w.deassign_task();
      }
    }

    // Assign available tasks to available workers
    for (const Task& task : tasks_in_order) {
      if (completed_tasks.end() != completed_tasks.find(task.get_name())) {
        continue; // Already done this task.
      }

      if (assigned_tasks.end() != assigned_tasks.find(task.get_name())) {
        continue; // Already being worked on
      }

      if (task.is_available(completed_tasks)) {
        for (Worker& w : workers) {
          if (!w.is_currently_assigned_task()) {
            w.assign_task(task, timer);
            assigned_tasks.insert(task.get_name());
            break;
          }
        }
      }
    }

    if (completed_tasks.size() != tasks_in_order.size()) {
      ++timer;
    }
  }

  return timer;
}
