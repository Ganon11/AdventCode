#include "pch.h"
#include "TreeNode.h"

std::unique_ptr<TreeNode> TreeNode::make_unique(const std::vector<unsigned int>& data)
{
   size_t index{ 0u };
   return make_unique(data, index);
}

std::vector<unsigned int> TreeNode::get_metadata() const
{
   return m_metadata;
}

unsigned int TreeNode::sum_metadata() const
{
   unsigned int sum{ 0 };
   for (const auto& child : m_children)
   {
      sum += child->sum_metadata();
   }

   for (const auto& metadata_entry : m_metadata)
   {
      sum += metadata_entry;
   }

   return sum;
}

unsigned int TreeNode::get_value() const
{
   unsigned int value{ 0 };
   if (m_children.size() == 0)
   {
      for (unsigned int metadata : m_metadata)
      {
         value += metadata;
      }

      return value;
   }

   for (unsigned int metadata : m_metadata)
   {
      if (metadata == 0)
      {
         continue;
      }

      unsigned int index = metadata - 1;
      if (index >= m_children.size())
      {
         continue;
      }

      value += m_children[index]->get_value();
   }

   return value;
}

std::unique_ptr<TreeNode> TreeNode::make_unique(const std::vector<unsigned int>& data, size_t& index)
{
   unsigned int num_children{ data[index] };
   unsigned int num_metadata{ data[index + 1] };
   index = index + 2;

   std::unique_ptr<TreeNode> node{ new TreeNode() };
   for (unsigned int child = 0; child < num_children; ++child)
   {
      node->m_children.push_back(make_unique(data, index));
   }

   for (unsigned int metadata = 0; metadata < num_metadata; ++metadata, ++index)
   {
      node->m_metadata.push_back(data[index]);
   }

   return node;
}

TreeNode::TreeNode()
{ }
