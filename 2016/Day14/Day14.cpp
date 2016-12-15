// Day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HashCache.h"
#include <chrono>
#include <iostream>
#include <string>

using namespace std::chrono;

int main() {
   auto functionStartTime{ high_resolution_clock::now() };
   //std::string salt{ "abc" }; // Sample
   std::string salt{ "ahsbgdzn" }; // Input

   size_t index = 0;
   unsigned int totalKeys = 0;
   HashCache cache;

   while (totalKeys < 64) {
      NodePtr node;
      if (cache.empty()) {
         node = std::make_shared<HashCacheNode>(salt, index);
      }
      else {
         node = cache.front();
         cache.pop_front();
      }

      size_t cacheSize{ cache.size() };

      if (node->HasTriple()) {
         char triple{ node->GetTriple() };

         for (size_t j = 0; j < 1000; ++j) {
            if (j >= cacheSize) {
               cache.push_back(std::make_shared<HashCacheNode>(salt, index + j + 1));
            }

            NodePtr quintupleNode{ cache[j] };
            if (quintupleNode->HasQuintuple(triple)) {
               ++totalKeys;
               break;
            }
         }
      }

      if (totalKeys < 64) {
         ++index;
      }
   }

   std::cout << "Found 64th key at index " << index << std::endl;

   auto functionEndTime{ high_resolution_clock::now() };
   auto functionDuration{ duration_cast<milliseconds>(functionEndTime - functionStartTime).count() };
   std::cout << "Finished, took " << functionDuration << " ms." << std::endl;

   return 0;
}
