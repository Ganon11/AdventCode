#include "conjunction_module.h"

#include <algorithm>
#include <iostream>

ConjunctionModule::ConjunctionModule(const std::string& name, std::queue<SignalToProcess>& the_queue)
  : Module{ name, the_queue }
{ }

void ConjunctionModule::receive_signal(const std::string& source, const SignalType signal)
{
  //std::cout << "Conjunction module " << name() << " receiving " << convert(signal) << " from " << source << std::endl;
  m_memory[source] = signal;

  // for (const auto& kvp : m_memory)
  // {
  //   std::cout << "\t" << kvp.first << ": " << kvp.second << std::endl;
  // }

  if (std::all_of(m_memory.begin(), m_memory.end(), [](const auto& kvp){ return kvp.second == HIGH; }))
  {
    send_signal(LOW);
  }
  else
  {
    send_signal(HIGH);
  }
}

void ConjunctionModule::add_source(const std::string& source)
{
  //std::cout << "Conjunction module " << name() << " adding input source " << source << std::endl;
  m_memory.insert(std::map<std::string, SignalType>::value_type{ source, LOW });
}
