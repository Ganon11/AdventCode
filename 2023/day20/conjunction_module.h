#pragma once

#include "module.h"

#include <map>
#include <string>

class ConjunctionModule : public Module
{
public:
  ConjunctionModule(const std::string& name, std::queue<SignalToProcess>& the_queue);

  virtual void receive_signal(const std::string& source, const SignalType signal) override;
  virtual void add_source(const std::string& source) override;

private:
  std::map<std::string, SignalType> m_memory;
};
