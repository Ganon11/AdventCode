#include "pch.h"
#include "Guard.h"

Guard::Guard()
{ }

Guard::Guard(unsigned int id) : m_id{ id }
{ }

void Guard::set_id(unsigned int id)
{
   m_id = id;
}

unsigned int Guard::get_id() const
{
   return m_id;
}

void Guard::add_sleep(unsigned int start_minute, unsigned int end_minute)
{
   for (unsigned int minute = start_minute; minute < end_minute; ++minute)
   {
      if (m_sleep_record.end() == m_sleep_record.find(minute))
      {
         m_sleep_record[minute] = 1u;
      }
      else
      {
         ++m_sleep_record[minute];
      }
   }
}

unsigned int Guard::minutes_asleep() const
{
   unsigned int sum = 0;
   for (const auto& minute : m_sleep_record)
   {
      sum += minute.second;
   }

   return sum;
}

unsigned int Guard::max_sleep_minute() const
{
   unsigned int currentMinute { 0 };
   unsigned int currentMax{ std::numeric_limits<unsigned int>::min() };
   for (const auto& minute : m_sleep_record)
   {
      if (minute.second > currentMax)
      {
         currentMinute = minute.first;
         currentMax = minute.second;
      }
   }

   return currentMinute;
}

unsigned int Guard::max_sleep_count() const
{
   unsigned int currentMax{ std::numeric_limits<unsigned int>::min() };
   for (const auto& minute : m_sleep_record)
   {
      if (minute.second > currentMax)
      {
         currentMax = minute.second;
      }
   }

   return currentMax;
}

bool Guard::operator==(const Guard& other) const
{
   return m_id == other.get_id();
}

bool Guard::operator==(const unsigned int id) const
{
   return m_id == id;
}
