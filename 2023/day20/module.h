#pragma once

#include <memory>
#include <queue>
#include <string>
#include <vector>

enum SignalType
{
  LOW,
  HIGH
};

std::string convert(const SignalType type);

struct SignalToProcess
{
  SignalType signal;
  std::string source;
  std::string destination;
};

class Module
{
public:
  Module(const std::string& name, std::queue<SignalToProcess>& the_queue);

  virtual ~Module() = default;

  virtual void receive_signal(const std::string& source, const SignalType signal);
  virtual void add_source(const std::string& source);
  virtual void add_destination(const std::string& destination);

  std::string name() const;
  std::vector<std::string> destinations() const;

protected:
  std::vector<std::string> m_destinations;

  void send_signal(const SignalType signal);

private:
  std::string m_name;
  std::queue<SignalToProcess>& m_the_queue;
};
