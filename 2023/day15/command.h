#pragma once

#include <string>

enum HASHMAPOperation
{
  REMOVAL,
  INSERTION
};

class HASHMAPCommand
{
public:
  HASHMAPCommand(const std::string& text);

  unsigned int target() const;
  std::string label() const;
  HASHMAPOperation operation() const;
  unsigned int focal_length() const;

  std::string to_string() const;

private:
  unsigned int m_target;
  std::string m_label;
  HASHMAPOperation m_op;
  unsigned int m_focal_length{0};
};
