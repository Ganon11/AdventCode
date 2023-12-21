#include "module.h"

#include <iostream>

std::string convert(const SignalType type)
{
  switch (type)
  {
  case LOW: return "low";
  case HIGH: return "high";
  default: return "???";
  }
}

Module::Module(const std::string& name, std::queue<SignalToProcess>& the_queue)
  : m_name{ name },
    m_the_queue{ the_queue }
{ }

std::string Module::name() const
{
  return m_name;
}

std::vector<std::string> Module::destinations() const
{
  return m_destinations;
}

void Module::receive_signal(const std::string& source, const SignalType signal)
{
  // Do nothing.
}

void Module::add_source(const std::string& source)
{ }

void Module::add_destination(const std::string& destination)
{
  m_destinations.push_back(destination);
}

void Module::send_signal(const SignalType signal)
{
  for (const auto& destination : m_destinations)
  {
    SignalToProcess new_signal{ signal, m_name, destination };
    m_the_queue.push(new_signal);
  }
}
