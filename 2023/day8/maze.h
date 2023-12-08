#pragma once

#include <string>

class MazeNode
{
public:
  MazeNode();
  MazeNode(const std::string& line);
  MazeNode(const MazeNode& other);
  MazeNode& operator=(const MazeNode& other);
  ~MazeNode() = default;

  std::string label() const;
  std::string left() const;
  std::string right() const;

  std::string to_string() const;

private:
  std::string m_label;
  std::string m_left;
  std::string m_right;
};
