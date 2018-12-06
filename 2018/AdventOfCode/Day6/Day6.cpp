// Day6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "Point.h"

using namespace std;

typedef vector<vector<int>> two_dimensional_map;

bool get_points(vector<Point>& points);
int closest_point(const Point& p, const vector<Point>& all_points);

vector<vector<int>> create_map(const vector<Point>& points);
int find_size_of_area(const two_dimensional_map& map, const int index);
int find_index_of_largest_area(const two_dimensional_map& map, const size_t num_points, int& largest_area);

int find_size_of_safe_area(const vector<Point>& points);
//bool write_map(const two_dimensional_map& map);

int main()
{
   vector<Point> points;
   if (!get_points(points))
   {
      wcout << L"Couldn't parse input." << endl;
      return -1;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Part 1
   /////////////////////////////////////////////////////////////////////////////
   const two_dimensional_map map{ create_map(points) };
   int largest_area;
   const int index_with_largest_area{ find_index_of_largest_area(map, points.size(), largest_area) };
   wcout << L"Largest Area - " << largest_area << endl;

   /////////////////////////////////////////////////////////////////////////////
   // Part 2
   /////////////////////////////////////////////////////////////////////////////
   const int safe_area = find_size_of_safe_area(points);
   wcout << L"Safe Area - " << safe_area << endl;

   return 0;
}

bool get_points(vector<Point>& points)
{
   wifstream input{ L"Input.txt" };
   if (!input.good())
   {
      return false;
   }

   wstring line;
   do
   {
      getline(input, line);
      points.push_back(line);
   } while (input.good());

   return true;
}

int closest_point(const Point& p, const vector<Point>& all_points)
{
   int min_distance{ numeric_limits<int>::max() };
   int min_distance_index;
   bool is_tied{ false };

   for (size_t index = 0; index < all_points.size(); ++index)
   {
      int distance{ p.DistanceTo(all_points[index]) };
      if (distance < min_distance)
      {
         min_distance = distance;
         min_distance_index = index;
         is_tied = false;
      }
      else if (distance == min_distance)
      {
         is_tied = true;
      }
   }

   if (is_tied)
   {
      return -1;
   }
   else
   {
      return min_distance_index;
   }
}

vector<vector<int>> create_map(const vector<Point>& points)
{
   int largest_x{ max_element(points.begin(), points.end(), [](const Point& p1, const Point& p2) { return p1.m_x < p2.m_x; })->m_x };
   int largest_y{ max_element(points.begin(), points.end(), [](const Point& p1, const Point& p2) { return p1.m_y < p2.m_y; })->m_y };
   vector<vector<int>> map;
   map.reserve(largest_y + 1);

   for (int row = 0; row <= largest_y; ++row)
   {
      vector<int> current_row;
      current_row.reserve(largest_x + 1);
      //map[row] = vector<int>{ static_cast<unsigned int>(largest_y + 1) };
      for (int col = 0; col <= largest_x; ++col)
      {
         Point p{ col, row };
         int closest{ closest_point(p, points) };
         current_row.push_back(closest);
      }

      map.push_back(current_row);
   }

   return map;
}

int find_size_of_area(const two_dimensional_map& map, const int index)
{
   int sum = 0;
   for (size_t row = 0; row < map.size(); ++row)
   {
      sum += count(map[row].begin(), map[row].end(), index);
   }

   return sum;
}

int find_index_of_largest_area(const two_dimensional_map& map, const size_t num_points, int& largest_area)
{
   set<int> infinite_areas;
   // Check top row (row = 0) and bottom row (row = map.size() - 1)
   for (size_t col = 0; col < map[0].size(); ++col)
   {
      infinite_areas.insert(map[0][col]);
      infinite_areas.insert(map[map.size() - 1][col]);
   }

   // Check left column (col = 0) and right column (col = map[0].size() - 1)
   for (size_t row = 0; row < map.size(); ++row)
   {
      infinite_areas.insert(map[row][0]);
      infinite_areas.insert(map[row][map[row].size() - 1]);
   }

   int max_area { numeric_limits<int>::min() };
   int largest_index;

   for (size_t index = 0; index < num_points; ++index)
   {
      if (infinite_areas.end() != infinite_areas.find(index))
      {
         // The point at this index has infinite area, and does not count.
         continue;
      }

      int area{ find_size_of_area(map, index) };
      if (area > max_area)
      {
         max_area = area;
         largest_index = index;
      }
   }

   largest_area = max_area;
   return largest_index;
}

namespace
{
   const int MAX_ALLOWED_DISTANCE{ 10000 };
}

int find_size_of_safe_area(const vector<Point>& points)
{
   int largest_x{ max_element(points.begin(), points.end(), [](const Point& p1, const Point& p2) { return p1.m_x < p2.m_x; })->m_x };
   int largest_y{ max_element(points.begin(), points.end(), [](const Point& p1, const Point& p2) { return p1.m_y < p2.m_y; })->m_y };

   int points_in_safe_area = 0;
   for (int row = 0; row <= largest_y; ++row)
   {
      for (int col = 0; col <= largest_x; ++col)
      {
         Point p{ col, row };
         int sum_distance{ 0 };
         for (size_t index = 0; index < points.size(); ++index)
         {
            sum_distance += p.DistanceTo(points[index]);
         }

         if (sum_distance < MAX_ALLOWED_DISTANCE)
         {
            ++points_in_safe_area;
         }
      }
   }

   return points_in_safe_area;
}

//bool write_map(const two_dimensional_map& map)
//{
//   wofstream outfile{ "Output.txt" };
//
//   if (!outfile.good())
//   {
//      return false;
//   }
//
//   for (size_t row = 0; row < map.size(); ++row)
//   {
//      for (size_t col = 0; col < map[row].size(); ++col)
//      {
//         if (map[row][col] == -1)
//         {
//            outfile << L"  .  ";
//         }
//         else
//         {
//            outfile << setw(3) << map[row][col] << L"  ";
//         }
//      }
//
//      outfile << endl;
//   }
//
//   return true;
//}
