#pragma once

#include "module.h"

class FlipFlopModule : public Module
{
public:
  FlipFlopModule(const std::string& name, std::queue<SignalToProcess>& the_queue);

  void receive_signal(const std::string& source, const SignalType signal) override;

private:
  bool m_status{ false };
};
