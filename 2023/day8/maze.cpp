#include "maze.h"

#include <iostream>
#include <regex>
#include <sstream>

MazeNode::MazeNode() : m_label{ "" }, m_left{ "" }, m_right{ "" }
{ }

MazeNode::MazeNode(const std::string& line)
{
  //std::cout << "Parsing \"" << line << "\"";
  const std::regex maze_node_regex{ "(\\w+) = \\((\\w+), (\\w+)\\)" };
  std::smatch match;
  std::regex_match(line, match, maze_node_regex);

  m_label = match[1];
  m_left = match[2];
  m_right = match[3];

  //std::cout << ", got label " << m_label << ", left " << m_left << ", right " << m_right << std::endl;
}

MazeNode::MazeNode(const MazeNode& other)
  : m_label{ other.m_label }, m_left{ other.m_left }, m_right{ other.m_right }
{ }

MazeNode& MazeNode::operator=(const MazeNode& other)
{
  m_label = other.m_label;
  m_left = other.m_left;
  m_right = other.m_right;

  return *this;
}

std::string MazeNode::label() const
{
  return m_label;
}

std::string MazeNode::left() const
{
  return m_left;
}

std::string MazeNode::right() const
{
  return m_right;
}

std::string MazeNode::to_string() const
{
  std::stringstream str;
  str << "Node \"" << m_label << "\", left to " << m_left << ", right to " << m_right;
  return str.str();
}
