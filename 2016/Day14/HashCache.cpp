#include "stdafx.h"
#include "HashCache.h"
#include "md5.h"
#include <sstream>

#define PART2

namespace {
   size_t ConvertCharToIndex(const char c) {
      switch (c) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
         return static_cast<size_t>(c - '0');
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
         return static_cast<size_t>(c - 'a' + 10);
      default:
         return 0xFFFF;
      }
   }
}

HashCacheNode::HashCacheNode() { }

HashCacheNode::HashCacheNode(const std::string& salt, const size_t index) {
   m_quintuples.resize(16, false);

   std::stringstream stream;
   stream << salt << index;
   std::string digest{ md5(stream.str()) };

#ifdef PART2
   for (int i = 0; i < 2016; ++i) {
      digest = md5(digest);
   }
#endif

   size_t i{ 0 }, end{ digest.size() };
   bool foundTriple{ false };
   m_triple = '\0';

   for (; i <= end - 5; ++i) {
      char c{ digest[i] };
      if (!foundTriple) {
         if (c == digest[i + 1] && c == digest[i + 2]) {
            m_triple = c;
            foundTriple = true;
         }
      }

      if (c == digest[i + 1] && c == digest[i + 2] && c == digest[i + 3] && c == digest[i + 4]) {
         m_quintuples[ConvertCharToIndex(digest[i])] = true;
      }
   }

   if (!foundTriple) {
      for (; i <= end - 3; ++i) {
         char c{ digest[i] };
         if (c == digest[i + 1] && c == digest[i + 2]) {
            m_triple = digest[i];
            break;
         }
      }
   }
}

bool HashCacheNode::HasTriple() const {
   return m_triple != '\0';
}

char HashCacheNode::GetTriple() const {
   return m_triple;
}

bool HashCacheNode::HasQuintuple(const char c) const {
   return m_quintuples[ConvertCharToIndex(c)];
}
