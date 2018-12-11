#pragma once

#include <map>

class Guard
{
public:
  Guard();
  Guard(unsigned int id);

  void set_id(unsigned int);
  unsigned int get_id() const;

  void add_sleep(unsigned int start_minute, unsigned int end_minute);
  unsigned int minutes_asleep() const;
  // Returns the minute of the hour that this guard sleeps the most
  unsigned int max_sleep_minute() const;
  // Returns the number of times this guard sleeps at that minute
  unsigned int max_sleep_count() const;

  bool operator==(const Guard& other) const;
  bool operator==(const unsigned int id) const;


private:
  unsigned int m_id;
  std::map<unsigned int, unsigned int> m_sleep_record;
};

namespace std {
  template<>
  struct hash<Guard>
  {
    typedef Guard argument_type;
    typedef unsigned int result_type;

    unsigned int operator()(const Guard& x) const {
      return x.get_id();
    }
  };
}
