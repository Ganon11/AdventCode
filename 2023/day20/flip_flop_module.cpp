#include "flip_flop_module.h"

FlipFlopModule::FlipFlopModule(const std::string& name, std::queue<SignalToProcess>& the_queue)
  : Module{ name, the_queue }
{ }

void FlipFlopModule::receive_signal(const std::string& source, const SignalType signal)
{
  if (signal == HIGH)
  {
    return;
  }

  if (!m_status)
  {
    m_status = true;
    send_signal(HIGH);
  }
  else
  {
    m_status = false;
    send_signal(LOW);
  }
}
