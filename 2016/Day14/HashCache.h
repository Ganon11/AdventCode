#pragma once

#include <string>
#include <vector>

class HashCacheNode {
public:
   HashCacheNode();
   HashCacheNode(const std::string& salt, const size_t index);

   bool HasTriple() const;
   char GetTriple() const;
   bool HasQuintuple(const char c) const;

private:
   char m_triple;
   std::vector<bool> m_quintuples;
};

typedef std::vector<HashCacheNode> HashCache;
