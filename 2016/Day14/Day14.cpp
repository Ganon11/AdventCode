// Day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HashCache.h"
#include <iostream>
#include <string>

int main() {
   //std::string salt{ "abc" }; // Sample
   std::string salt{ "ahsbgdzn" }; // Input

   size_t index = 0;
   unsigned int totalKeys = 0;
   HashCache cache;
   cache.push_back(HashCacheNode{ salt, index });

   while (totalKeys < 64) {
      ++index;
      size_t cacheSize{ cache.size() };
      HashCacheNode node;
      if (cacheSize == 0) {
         node = HashCacheNode(salt, index);
      }
      else {
         node = cache[0];
         cache.erase(std::begin(cache));
         --cacheSize;
      }

      if (node.HasTriple()) {
         char triple{ node.GetTriple() };
         for (size_t j = 0; j < 1000; ++j) {
            if (j >= cacheSize) {
               cache.push_back(HashCacheNode(salt, index + j + 1));
            }
            HashCacheNode quintupleNode{ cache[j] };

            if (quintupleNode.HasQuintuple(triple)) {
               ++totalKeys;
               break;
            }
         }
      }
   }

   std::cout << "Found 64th key at index " << index << std::endl;
   return 0;
}
