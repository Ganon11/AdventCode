#include "command.h"
#include "hash.h"

#include <sstream>

HASHMAPCommand::HASHMAPCommand(const std::string& text)
{
  size_t position{ text.find('=') };
  if (std::string::npos == position)
  {
    m_op = REMOVAL;
    m_label = text.substr(0, text.size() - 1);
  }
  else
  {
    m_op = INSERTION;
    m_label = text.substr(0, position);
    m_focal_length = text[position + 1] - '0';
  }

  m_target = hash(m_label);
}

unsigned int HASHMAPCommand::target() const
{
  return m_target;
}

std::string HASHMAPCommand::label() const
{
  return m_label;
}

HASHMAPOperation HASHMAPCommand::operation() const
{
  return m_op;
}

unsigned int HASHMAPCommand::focal_length() const
{
  return m_focal_length;
}

std::string HASHMAPCommand::to_string() const
{
  std::stringstream str;
  str << m_label;
  switch (m_op)
  {
  case INSERTION:
    str << "=" << m_focal_length;
    break;
  case REMOVAL:
    str << "-";
    break;
  }

  return str.str();
}
