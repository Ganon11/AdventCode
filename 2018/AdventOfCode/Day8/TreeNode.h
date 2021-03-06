#pragma once

#include <memory>
#include <vector>

typedef std::vector<unsigned int> DataVector;

class TreeNode
{
public:
  std::vector<unsigned int> get_metadata() const;
  unsigned int sum_metadata() const;
  unsigned int get_value() const;

  static std::unique_ptr<TreeNode> make_unique(const DataVector& data);

private:
  TreeNode();
  static std::unique_ptr<TreeNode> make_unique(const DataVector& data, size_t& index);

  std::vector<std::unique_ptr<TreeNode>> m_children;
  std::vector<unsigned int> m_metadata;
};
