#pragma once

#include "module.h"

class BroadcastModule : public Module
{
public:
  BroadcastModule(const std::string& name, std::queue<SignalToProcess>& the_queue);

  virtual void receive_signal(const std::string& source, const SignalType signal) override;

private:
};
