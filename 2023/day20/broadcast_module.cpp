#include "broadcast_module.h"

BroadcastModule::BroadcastModule(const std::string& name, std::queue<SignalToProcess>& the_queue)
  : Module{ name, the_queue }
{ }

void BroadcastModule::receive_signal(const std::string& source, const SignalType signal)
{
  send_signal(signal);
}
