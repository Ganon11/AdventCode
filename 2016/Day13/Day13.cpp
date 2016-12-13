// Day13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Point.h"

#include <deque>
#include <iostream>
#include <map>
#include <string>

#ifdef _DEBUG
#define DESTINATION_X 7
#define DESTINATION_Y 4
#else
#define DESTINATION_X 31
#define DESTINATION_Y 39
#endif

int wmain(int argc, wchar_t *argv[]) {
   bool part1{ true };

   if (argc == 2) {
      std::wstring arg{ argv[1] };
      part1 = L"2" != arg;
   }

   Point start{ 1, 1 };
   std::map<Point, unsigned int> distanceMap;
   std::deque<Point> queue;

   distanceMap[start] = 0;
   queue.push_back(start);

   while (!queue.empty()) {
      const Point p{ queue.front() };
      queue.pop_front();
      unsigned int distance{ distanceMap[p] };

      if (part1 && p.x == DESTINATION_X && p.y == DESTINATION_Y) {
         std::wcout << L"Found it in " << distance << L" steps." << std::endl;
         break;
      }

      if (!part1 && distance >= 50) {
         continue;
      }

      // Move up
      if (p.y > 0) {
         Point newPoint{ p.x, p.y - 1 };
         if (newPoint.IsOpen() && std::end(distanceMap) == distanceMap.find(newPoint)) {
            distanceMap.insert(std::make_pair(newPoint, distance + 1));
            queue.push_back(newPoint);
         }
      }

      // Move down
      {
         Point newPoint{ p.x, p.y + 1 };
         if (newPoint.IsOpen() && std::end(distanceMap) == distanceMap.find(newPoint)) {
            distanceMap.insert(std::make_pair(newPoint, distance + 1));
            queue.push_back(newPoint);
         }
      }

      // Move left
      if (p.x > 0) {
         Point newPoint{ p.x - 1, p.y };
         if (newPoint.IsOpen() && std::end(distanceMap) == distanceMap.find(newPoint)) {
            distanceMap.insert(std::make_pair(newPoint, distance + 1));
            queue.push_back(newPoint);
         }
      }

      // Move right
      {
         Point newPoint{ p.x + 1, p.y };
         if (newPoint.IsOpen() && std::end(distanceMap) == distanceMap.find(newPoint)) {
            distanceMap.insert(std::make_pair(newPoint, distance + 1));
            queue.push_back(newPoint);
         }
      }
   }

   std::wcout << L"Visited " << distanceMap.size() << L" unique locations." << std::endl;
   return 0;
}
